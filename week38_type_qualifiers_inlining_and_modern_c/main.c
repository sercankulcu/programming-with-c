#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <setjmp.h>
#include <time.h>

#define N 2000000
#define REPS 50

static double now(void)
{
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return (double)t.tv_sec + (double)t.tv_nsec / 1e9;
}

/* ---------- restrict ---------- */

static void add_plain(int *dst, const int *a, const int *b, size_t n)
{
    for (size_t i = 0; i < n; i++) dst[i] = a[i] + b[i];
}

static void add_restrict(int *restrict dst, const int *restrict a,
                         const int *restrict b, size_t n)
{
    for (size_t i = 0; i < n; i++) dst[i] = a[i] + b[i];
}

/* ---------- static inline replaces a macro ---------- */

#define MAX_MACRO(a, b) ((a) > (b) ? (a) : (b))     /* evaluates twice */

static inline int max_int(int a, int b)             /* evaluates once */
{
    return a > b ? a : b;
}

static int side_effect_count = 0;
static int with_side_effect(int v)
{
    side_effect_count++;
    return v;
}

/* ---------- compound literals ---------- */

typedef struct { int x, y; } Point;

static int manhattan(Point a, Point b)
{
    int dx = a.x - b.x, dy = a.y - b.y;
    return (dx < 0 ? -dx : dx) + (dy < 0 ? -dy : dy);
}

/* ---------- anonymous union ---------- */

typedef enum { V_INT, V_DOUBLE, V_TEXT } VKind;

typedef struct {
    VKind kind;
    union {                            /* anonymous: C11 */
        int    as_int;
        double as_double;
        char   as_text[16];
    };
} Value;

static void value_print(const Value *v)
{
    switch (v->kind) {
    case V_INT:    printf("  int    %d\n", v->as_int);       break;
    case V_DOUBLE: printf("  double %g\n", v->as_double);    break;
    case V_TEXT:   printf("  text   \"%s\"\n", v->as_text);  break;
    }
}

/* ---------- _Noreturn ---------- */

_Noreturn static void fatal(const char *msg)
{
    fprintf(stderr, "fatal: %s\n", msg);
    exit(EXIT_FAILURE);
}

/* ---------- setjmp / longjmp ---------- */

static jmp_buf recovery;

static void level_three(int depth)
{
    printf("    level three, depth %d\n", depth);
    if (depth > 2) {
        puts("    jumping back, skipping every frame between");
        longjmp(recovery, depth);       /* no cleanup runs */
    }
}
static void level_two(int depth)   { printf("   level two\n");  level_three(depth); }
static void level_one(int depth)   { printf("  level one\n");   level_two(depth); }

int main(void)
{
    puts("== restrict: the same loop, two contracts ==");
    int *a   = malloc(N * sizeof *a);
    int *b   = malloc(N * sizeof *b);
    int *dst = malloc(N * sizeof *dst);
    if (a == NULL || b == NULL || dst == NULL) fatal("out of memory");

    for (size_t i = 0; i < N; i++) { a[i] = (int)i; b[i] = (int)(i * 2); }

    double t = now();
    for (int r = 0; r < REPS; r++) add_plain(dst, a, b, N);
    double plain_secs = now() - t;

    t = now();
    for (int r = 0; r < REPS; r++) add_restrict(dst, a, b, N);
    double restrict_secs = now() - t;

    printf("  without restrict : %.4f s\n", plain_secs);
    printf("  with restrict    : %.4f s\n", restrict_secs);
    printf("  ratio %.2fx (check the assembly for vector instructions)\n",
           plain_secs / restrict_secs);
    printf("  last element %d, both versions agree\n", dst[N - 1]);

    puts("\n== static inline versus a macro ==");
    side_effect_count = 0;
    int m1 = MAX_MACRO(with_side_effect(3), with_side_effect(5));
    printf("  MAX_MACRO gave %d after %d calls  <-- evaluated twice\n",
           m1, side_effect_count);

    side_effect_count = 0;
    int m2 = max_int(with_side_effect(3), with_side_effect(5));
    printf("  max_int   gave %d after %d calls  <-- evaluated once\n",
           m2, side_effect_count);

    puts("\n== compound literals ==");
    printf("  manhattan((0,0),(3,4)) = %d   no temporaries declared\n",
           manhattan((Point){ .x = 0, .y = 0 }, (Point){ .x = 3, .y = 4 }));

    int *fib = (int[]){ 1, 1, 2, 3, 5, 8, 13 };
    printf("  an unnamed array: ");
    for (int i = 0; i < 7; i++) printf("%d ", fib[i]);
    puts("\n  it lives until the end of this block, and no longer");

    puts("\n== anonymous union ==");
    Value values[] = {
        { .kind = V_INT,    .as_int    = 42 },
        { .kind = V_DOUBLE, .as_double = 3.14 },
        { .kind = V_TEXT,   .as_text   = "hello" }
    };
    for (size_t i = 0; i < 3; i++) value_print(&values[i]);
    printf("  accessed as v->as_int, not v->data.as_int\n");

    puts("\n== volatile ==");
    int          normal = 0;
    volatile int observed = 0;
    for (int i = 0; i < 3; i++) { normal++; observed++; }
    printf("  normal = %d, volatile = %d\n", normal, observed);
    puts("  at -O2 the first may be computed at compile time;");
    puts("  the volatile one must be loaded and stored three times");
    puts("  volatile does NOT make ++ atomic — that is week 41");

    puts("\n== setjmp and longjmp ==");
    int landed = setjmp(recovery);
    if (landed == 0) {
        puts("  first pass, descending:");
        level_one(3);
        puts("  never reached");
    } else {
        printf("  back in main via longjmp with value %d\n", landed);
        puts("  three frames were discarded and no cleanup ran");
        puts("  anything they allocated has leaked");
    }

    puts("\n== why VLAs are avoided ==");
    size_t safe_size = 100;
    printf("  a VLA of %zu ints would be fine\n", safe_size);
    puts("  a VLA of a size taken from input cannot report failure:");
    puts("  too large and the process dies with no way to catch it");
    int *heap = malloc(safe_size * sizeof *heap);
    printf("  malloc can: %s\n", heap ? "allocation checked" : "refused");
    free(heap);

    free(a); free(b); free(dst);
    return EXIT_SUCCESS;
}
