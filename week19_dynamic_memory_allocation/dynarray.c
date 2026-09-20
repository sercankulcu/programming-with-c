#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
    int    *data;
    size_t  count;      /* elements in use   */
    size_t  capacity;   /* elements allocated */
} IntArray;

/* Paired with array_destroy. After this returns true the caller owns
   the array and must call array_destroy exactly once. */
static bool array_init(IntArray *a, size_t initial_capacity)
{
    if (initial_capacity == 0) {
        initial_capacity = 4;
    }
    a->data = malloc(initial_capacity * sizeof *a->data);
    if (a->data == NULL) {
        a->count = a->capacity = 0;
        return false;
    }
    a->count = 0;
    a->capacity = initial_capacity;
    return true;
}

static void array_destroy(IntArray *a)
{
    free(a->data);              /* free(NULL) is safe */
    a->data = NULL;
    a->count = a->capacity = 0;
}

/* Doubling gives amortized O(1) append: n appends cost O(n) copies total. */
static bool array_grow(IntArray *a)
{
    size_t new_capacity = a->capacity * 2;

    int *bigger = realloc(a->data, new_capacity * sizeof *a->data);
    if (bigger == NULL) {
        return false;           /* a->data is still valid and still ours */
    }
    a->data = bigger;
    a->capacity = new_capacity;
    return true;
}

static bool array_push(IntArray *a, int value)
{
    if (a->count == a->capacity && !array_grow(a)) {
        return false;
    }
    a->data[a->count++] = value;
    return true;
}

static bool array_get(const IntArray *a, size_t index, int *out)
{
    if (index >= a->count) {
        return false;
    }
    *out = a->data[index];
    return true;
}

static void array_print(const IntArray *a, const char *label)
{
    printf("  %-10s count=%-3zu capacity=%-3zu [", label, a->count, a->capacity);
    for (size_t i = 0; i < a->count; i++) {
        printf("%d%s", a->data[i], i + 1 < a->count ? " " : "");
    }
    puts("]");
}

/* Caller frees the result. Named so the obligation is hard to miss. */
static char *duplicate_string(const char *s)
{
    size_t bytes = strlen(s) + 1;          /* room for the terminator */
    char *copy = malloc(bytes);
    if (copy == NULL) {
        return NULL;
    }
    memcpy(copy, s, bytes);
    return copy;
}

int main(void)
{
    puts("== growable array ==");
    IntArray a;
    if (!array_init(&a, 2)) {
        fprintf(stderr, "out of memory\n");
        return EXIT_FAILURE;
    }

    array_print(&a, "empty");
    for (int i = 1; i <= 9; i++) {
        if (!array_push(&a, i * i)) {
            fprintf(stderr, "out of memory\n");
            array_destroy(&a);
            return EXIT_FAILURE;
        }
        if (a.count == a.capacity) {
            array_print(&a, "full");
        }
    }
    array_print(&a, "final");
    puts("  capacity doubled 2 -> 4 -> 8 -> 16: five reallocations for nine pushes");

    int value;
    printf("  element 3  : %s", array_get(&a, 3, &value) ? "" : "refused\n");
    if (array_get(&a, 3, &value)) printf("%d\n", value);
    printf("  element 99 : %s\n",
           array_get(&a, 99, &value) ? "returned a value" : "refused");

    array_destroy(&a);
    puts("  destroyed; calling array_destroy again would also be safe");
    array_destroy(&a);

    puts("\n== calloc zeroes, malloc does not ==");
    int *raw = malloc(4 * sizeof *raw);
    int *zeroed = calloc(4, sizeof *zeroed);
    if (raw != NULL && zeroed != NULL) {
        printf("  malloc: %d %d %d %d   (garbage — do not rely on it)\n",
               raw[0], raw[1], raw[2], raw[3]);
        printf("  calloc: %d %d %d %d   (guaranteed zero)\n",
               zeroed[0], zeroed[1], zeroed[2], zeroed[3]);
    }
    free(raw);
    free(zeroed);

    puts("\n== a flat matrix: one allocation, one free ==");
    const size_t rows = 3, cols = 4;
    int *m = malloc(rows * cols * sizeof *m);
    if (m == NULL) {
        return EXIT_FAILURE;
    }
    for (size_t r = 0; r < rows; r++) {
        for (size_t c = 0; c < cols; c++) {
            m[r * cols + c] = (int)(r * cols + c);
        }
    }
    for (size_t r = 0; r < rows; r++) {
        printf("  ");
        for (size_t c = 0; c < cols; c++) {
            printf("%3d", m[r * cols + c]);
        }
        putchar('\n');
    }
    free(m);

    puts("\n== ownership, stated in the name ==");
    char *copy = duplicate_string("the caller frees this");
    if (copy != NULL) {
        printf("  \"%s\"\n", copy);
        free(copy);
    }

    return EXIT_SUCCESS;
}
