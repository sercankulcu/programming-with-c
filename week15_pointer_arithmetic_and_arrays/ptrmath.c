#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

/* Index style. */
static long sum_indexed(const int *values, size_t n)
{
    long total = 0;
    for (size_t i = 0; i < n; i++) {
        total += values[i];
    }
    return total;
}

/* Pointer style: identical machine code on any modern compiler. */
static long sum_pointer(const int *values, size_t n)
{
    long total = 0;
    for (const int *q = values; q < values + n; q++) {
        total += *q;
    }
    return total;
}

/* Pointer style where it genuinely fits: no count is available,
   the terminator ends the walk. */
static size_t length_of(const char *s)
{
    const char *start = s;
    while (*s != '\0') {
        s++;
    }
    return (size_t)(s - start);
}

/* Reverse traversal done safely: never forms values - 1. */
static void print_backwards(const int *values, size_t n)
{
    for (const int *q = values + n; q-- > values; ) {
        printf("%d ", *q);
    }
    putchar('\n');
}

/* An output parameter that is itself a pointer. */
static void point_at_largest(int *values, size_t n, int **out)
{
    if (n == 0) {
        *out = NULL;
        return;
    }
    int *best = values;
    for (int *q = values + 1; q < values + n; q++) {
        if (*q > *best) {
            best = q;
        }
    }
    *out = best;
}

int main(void)
{
    int values[] = { 10, 20, 30, 40, 50 };
    const size_t n = sizeof values / sizeof values[0];
    int *p = values;

    puts("== scaling ==");
    printf("  sizeof(int)        = %zu\n", sizeof(int));
    printf("  p                  = %p\n", (void *)p);
    printf("  p + 1              = %p   (+%td bytes)\n",
           (void *)(p + 1), (char *)(p + 1) - (char *)p);
    printf("  p + 3              = %p   (+%td bytes)\n",
           (void *)(p + 3), (char *)(p + 3) - (char *)p);

    puts("\n== the four spellings of one element ==");
    printf("  values[2]=%d  *(values+2)=%d  p[2]=%d  *(p+2)=%d\n",
           values[2], *(values + 2), p[2], *(p + 2));

    puts("\n== subtraction gives elements, not bytes ==");
    int *first = &values[1];
    int *last  = &values[4];
    printf("  &values[4] - &values[1] = %td elements\n", last - first);

    puts("\n== three traversals, one answer ==");
    printf("  sum_indexed = %ld\n", sum_indexed(values, n));
    printf("  sum_pointer = %ld\n", sum_pointer(values, n));
    printf("  backwards:    ");
    print_backwards(values, n);

    puts("\n== pointer style where it belongs ==");
    const char *text = "pointer arithmetic";
    printf("  length_of(\"%s\") = %zu   strlen = %zu\n",
           text, length_of(text), strlen(text));

    puts("\n== array of pointers ==");
    const char *names[] = { "Ada", "Dennis", "Ken", "Bjarne" };
    const size_t names_count = sizeof names / sizeof names[0];
    printf("  the array holds %zu pointers, %zu bytes total\n",
           names_count, sizeof names);
    for (size_t i = 0; i < names_count; i++) {
        printf("    names[%zu] -> \"%s\" (%zu chars)\n",
               i, names[i], strlen(names[i]));
    }

    puts("\n== pointer to pointer as an output parameter ==");
    int *largest = NULL;
    point_at_largest(values, n, &largest);
    if (largest != NULL) {
        printf("  largest value %d is at index %td\n",
               *largest, largest - values);
    }

    puts("\n== one past the end ==");
    const int *end = values + n;
    printf("  values + %zu is a legal address to form: %p\n", n, (const void *)end);
    puts("  dereferencing it would be undefined behavior");

    return EXIT_SUCCESS;
}
