#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- 1. the overflow check that disappears ---------- */

static bool add_checked_broken(int a, int b, int *out)
{
    int sum = a + b;               /* UB on overflow */
    if (sum < a) {                 /* assumes wrapping; may be deleted */
        return false;
    }
    *out = sum;
    return true;
}

static bool add_checked(int a, int b, int *out)
{
    if (b > 0 && a > INT_MAX - b) return false;   /* checked BEFORE */
    if (b < 0 && a < INT_MIN - b) return false;
    *out = a + b;
    return true;
}

/* ---------- 2. the null check that disappears ---------- */

static int deref_then_check(int *p)
{
    int value = *p;                /* if p were NULL this is UB ... */
    if (p == NULL) {               /* ... so the compiler may delete this */
        return -1;
    }
    return value;
}

static int check_then_deref(int *p)
{
    if (p == NULL) return -1;      /* the only order that works */
    return *p;
}

/* ---------- 3. strict aliasing ---------- */

static uint32_t bits_violating(float f)
{
    return *(uint32_t *)&f;        /* undefined behavior */
}

static uint32_t bits_memcpy(float f)
{
    uint32_t bits;
    memcpy(&bits, &f, sizeof bits);   /* correct, and free at -O2 */
    return bits;
}

static uint32_t bits_union(float f)
{
    union { float f; uint32_t u; } conv = { .f = f };
    return conv.u;                    /* correct in C */
}

/* ---------- 4. the allocation-size overflow ---------- */

typedef struct { int id; char pad[44]; } Record;   /* 48 bytes */

static Record *allocate_broken(size_t count)
{
    return malloc(count * sizeof(Record));         /* can wrap */
}

static Record *allocate_checked(size_t count)
{
    if (count > SIZE_MAX / sizeof(Record)) {
        return NULL;                               /* refuse */
    }
    return calloc(count, sizeof(Record));          /* checks as well */
}

/* ---------- 5. format strings ---------- */

static void print_broken(const char *user)
{
    printf(user);                  /* -Wformat-security warns */
    putchar('\n');
}

static void print_safe(const char *user)
{
    printf("%s\n", user);
}

int main(void)
{
    printf("built at optimization level: ");
#ifdef __OPTIMIZE__
    puts("optimized");
#else
    puts("-O0");
#endif

    puts("\n== 1. signed overflow ==");
    int result = 0;
    printf("  broken check, INT_MAX + 1 : %s\n",
           add_checked_broken(INT_MAX, 1, &result) ? "ACCEPTED (wrong)"
                                                   : "rejected");
    printf("  correct check, INT_MAX + 1: %s\n",
           add_checked(INT_MAX, 1, &result) ? "ACCEPTED (wrong)" : "rejected");
    printf("  correct check, 100 + 200  : %s (%d)\n",
           add_checked(100, 200, &result) ? "accepted" : "rejected", result);
    puts("  compile at -O2 and compare: the broken check may vanish");

    puts("\n== 2. null checks ==");
    int value = 42;
    printf("  check_then_deref(&value) = %d\n", check_then_deref(&value));
    printf("  check_then_deref(NULL)   = %d\n", check_then_deref(NULL));
    puts("  deref_then_check(NULL) is NOT called here: at -O2 the test");
    puts("  may be gone and it would simply crash");

    puts("\n== 3. strict aliasing ==");
    float f = 1.0f;
    printf("  memcpy : 0x%08X\n", bits_memcpy(f));
    printf("  union  : 0x%08X\n", bits_union(f));
    printf("  cast   : 0x%08X  <-- undefined behavior, works by luck\n",
           bits_violating(f));
    puts("  all three agree today; only two are guaranteed to");

    puts("\n== 4. allocation size overflow ==");
    size_t huge = SIZE_MAX / 48 + 2;
    printf("  requesting %zu records of %zu bytes\n", huge, sizeof(Record));
    printf("  count * sizeof wraps to %zu bytes\n", huge * sizeof(Record));
    Record *bad = allocate_broken(huge);
    printf("  malloc(wrapped) : %s  <-- a tiny buffer for a huge count\n",
           bad ? "SUCCEEDED" : "failed");
    free(bad);
    Record *good = allocate_checked(huge);
    printf("  checked version : %s\n", good ? "succeeded" : "refused");
    free(good);

    puts("\n== 5. format strings ==");
    print_safe("a normal message");
    print_safe("%s %s %s %n");     /* harmless: it is just data */
    puts("  the same string passed to printf() directly would read");
    puts("  arguments that were never pushed, and %n would WRITE");

    puts("\n== 6. shifting too far ==");
    unsigned shift_by = 32;
    uint32_t one = 1;
    printf("  1u << 32 on a 32-bit type is undefined\n");
    if (shift_by < 32) {           /* the guard that makes it defined */
        printf("  guarded: %u\n", one << shift_by);
    } else {
        puts("  guarded: refused, as it must be");
    }

    return EXIT_SUCCESS;
}
