#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* Week 11's version: swaps two copies, achieves nothing. */
static void swap_broken(int a, int b)
{
    int temp = a;
    a = b;
    b = temp;
}

/* The fix: three asterisks and two ampersands at the call site. */
static void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

/* An output parameter: return a status, write the result through a pointer.
   This is the convention week 16 develops. */
static bool safe_divide(int numerator, int denominator, int *out)
{
    if (denominator == 0) {
        return false;
    }
    *out = numerator / denominator;
    return true;
}

/* Demonstrates decay: sizeof here reports a pointer, not the array. */
static void show_parameter_size(int values[], size_t n)
{
    printf("  inside the function: sizeof values = %zu  (a pointer)\n",
           sizeof values);
    printf("  the length had to be passed separately: n = %zu\n", n);
}

/* Writes through the decayed pointer, so the caller's array changes. */
static void scale_all(int values[], size_t n, int factor)
{
    for (size_t i = 0; i < n; i++) {
        values[i] *= factor;
    }
}

int main(void)
{
    puts("== 1. addresses ==");
    int count = 42;
    int *p = &count;
    printf("  count      = %d\n", count);
    printf("  &count     = %p\n", (void *)&count);
    printf("  p          = %p\n", (void *)p);
    printf("  *p         = %d\n", *p);
    *p = 99;
    printf("  after *p = 99, count = %d\n", count);
    printf("  sizeof(int) = %zu, sizeof(int *) = %zu\n",
           sizeof(int), sizeof(int *));

    puts("\n== 2. swap, broken and fixed ==");
    int x = 1, y = 2;
    swap_broken(x, y);
    printf("  after swap_broken: x=%d y=%d   (unchanged)\n", x, y);
    swap(&x, &y);
    printf("  after swap:        x=%d y=%d   (swapped)\n", x, y);

    puts("\n== 3. output parameters ==");
    int result;
    if (safe_divide(10, 3, &result)) {
        printf("  10 / 3 = %d\n", result);
    }
    if (!safe_divide(10, 0, &result)) {
        puts("  10 / 0 refused, and result was left untouched");
    }

    puts("\n== 4. null and guarded access ==");
    int *maybe = NULL;
    printf("  maybe is %s\n", maybe == NULL ? "NULL" : "valid");
    if (maybe != NULL) {
        printf("  %d\n", *maybe);
    } else {
        puts("  guarded: no dereference attempted");
    }

    puts("\n== 5. array decay ==");
    int values[5] = { 1, 2, 3, 4, 5 };
    const size_t n = sizeof values / sizeof values[0];
    printf("  where declared: sizeof values = %zu, length = %zu\n",
           sizeof values, n);
    show_parameter_size(values, n);

    printf("  values == &values[0]?  %s\n",
           (void *)values == (void *)&values[0] ? "yes" : "no");
    printf("  *values = %d, values[0] = %d, *(values + 2) = %d, values[2] = %d\n",
           *values, values[0], *(values + 2), values[2]);

    puts("\n== 6. a function modifying the caller's array ==");
    printf("  before: ");
    for (size_t i = 0; i < n; i++) printf("%d ", values[i]);
    scale_all(values, n, 10);
    printf("\n  after:  ");
    for (size_t i = 0; i < n; i++) printf("%d ", values[i]);
    puts("\n  pass-by-value was never broken: the pointer was copied");

    return EXIT_SUCCESS;
}
