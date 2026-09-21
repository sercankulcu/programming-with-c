#include "intarray.h"

#include <stdio.h>
#include <stdlib.h>

/* A debug macro that reports the expression it evaluated. */
#define TRACE(expr) \
    fprintf(stderr, "%s:%d: %s = %zu\n", __FILE__, __LINE__, #expr, (size_t)(expr))

int main(void)
{
    printf("compiled against C standard %ld\n", __STDC_VERSION__);

    IntArray *a = intarray_create(2);
    if (a == NULL) {
        fprintf(stderr, "out of memory\n");
        return EXIT_FAILURE;
    }

    for (int i = 1; i <= 9; i++) {
        if (!intarray_push(a, i * i)) {
            fprintf(stderr, "push failed\n");
            intarray_destroy(a);
            return EXIT_FAILURE;
        }
    }

    printf("count = %zu, capacity = %zu\n",
           intarray_count(a), intarray_capacity(a));

    printf("contents:");
    for (size_t i = 0; i < intarray_count(a); i++) {
        int value;
        if (intarray_get(a, i, &value)) {
            printf(" %d", value);
        }
    }
    putchar('\n');

    int value;
    printf("index 99 -> %s\n",
           intarray_get(a, 99, &value) ? "value" : "refused");

    TRACE(intarray_count(a));

    /* Neither of these compiles — and that is the point:
     *   IntArray local;          // incomplete type
     *   printf("%zu", a->count); // dereferencing an incomplete type
     */

    intarray_destroy(a);
    intarray_destroy(NULL);        /* documented as safe */
    return EXIT_SUCCESS;
}
