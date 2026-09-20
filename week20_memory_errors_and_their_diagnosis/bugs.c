#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Build with -DBUG=1 .. -DBUG=5 to enable one defect at a time.
   Build with no -DBUG for the clean version. */
#ifndef BUG
#define BUG 0
#endif

typedef struct {
    int    *data;
    size_t  count;
    size_t  capacity;
} IntArray;

static bool array_init(IntArray *a, size_t capacity)
{
    a->data = malloc(capacity * sizeof *a->data);
    if (a->data == NULL) {
        return false;
    }
    a->count = 0;
    a->capacity = capacity;
    return true;
}

static void array_destroy(IntArray *a)
{
    free(a->data);
    a->data = NULL;            /* the habit that prevents bugs 2 and 3 */
    a->count = a->capacity = 0;
}

static bool array_push(IntArray *a, int value)
{
    if (a->count == a->capacity) {
        size_t bigger_capacity = a->capacity * 2;
        int *bigger = realloc(a->data, bigger_capacity * sizeof *bigger);
        if (bigger == NULL) {
            return false;
        }
        a->data = bigger;
        a->capacity = bigger_capacity;
    }
    a->data[a->count++] = value;
    return true;
}

int main(void)
{
    printf("built with BUG=%d\n\n", BUG);

    IntArray a;
    if (!array_init(&a, 4)) {
        return EXIT_FAILURE;
    }
    for (int i = 1; i <= 6; i++) {
        array_push(&a, i * 10);
    }
    printf("array holds %zu of %zu: ", a.count, a.capacity);
    for (size_t i = 0; i < a.count; i++) printf("%d ", a.data[i]);
    putchar('\n');

#if BUG == 1
    /* LEAK: allocate and forget. */
    char *forgotten = malloc(64);
    strcpy(forgotten, "nobody will free me");
    printf("leaked: %s\n", forgotten);
    /* no free */

#elif BUG == 2
    /* USE AFTER FREE. */
    free(a.data);
    printf("after free, a.data[0] = %d\n", a.data[0]);
    a.data = NULL;

#elif BUG == 3
    /* DOUBLE FREE. */
    free(a.data);
    free(a.data);
    a.data = NULL;

#elif BUG == 4
    /* HEAP OVERRUN: one element past the end. */
    printf("writing a.data[%zu] with capacity %zu\n", a.capacity, a.capacity);
    a.data[a.capacity] = 999;

#elif BUG == 5
    /* INVALID FREE: not the pointer malloc returned. */
    free(a.data + 1);
    a.data = NULL;
#endif

    array_destroy(&a);
    puts("finished");
    return EXIT_SUCCESS;
}
