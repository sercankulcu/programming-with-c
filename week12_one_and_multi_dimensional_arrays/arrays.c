#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define ROWS 3
#define COLS 4

static void print_matrix(const char *label, size_t rows, size_t cols,
                         const int m[rows][cols])
{
    printf("%s (%zux%zu)\n", label, rows, cols);
    for (size_t r = 0; r < rows; r++) {
        printf("  ");
        for (size_t c = 0; c < cols; c++) {
            printf("%4d", m[r][c]);
        }
        putchar('\n');
    }
}

static void transpose(size_t rows, size_t cols,
                      const int src[rows][cols], int dst[cols][rows])
{
    for (size_t r = 0; r < rows; r++) {
        for (size_t c = 0; c < cols; c++) {
            dst[c][r] = src[r][c];
        }
    }
}

/* Bounds-checked access. Returns false instead of reading out of range. */
static bool at(const int values[], size_t n, size_t index, int *out)
{
    if (index >= n) {
        return false;
    }
    *out = values[index];
    return true;
}

static int sum(const int values[], size_t n)
{
    int total = 0;
    for (size_t i = 0; i < n; i++) {
        total += values[i];
    }
    return total;
}

static void reverse(int values[], size_t n)
{
    for (size_t i = 0; i < n / 2; i++) {
        int temp = values[i];
        values[i] = values[n - 1 - i];
        values[n - 1 - i] = temp;
    }
}

static size_t find(const int values[], size_t n, int target)
{
    for (size_t i = 0; i < n; i++) {
        if (values[i] == target) {
            return i;
        }
    }
    return n;                          /* n means "not found" */
}

int main(void)
{
    int m[ROWS][COLS] = {
        {  1,  2,  3,  4 },
        {  5,  6,  7,  8 },
        {  9, 10, 11, 12 }
    };
    int t[COLS][ROWS];

    print_matrix("original", ROWS, COLS, m);
    transpose(ROWS, COLS, m, t);
    print_matrix("transposed", COLS, ROWS, t);

    puts("\nrow-major layout: the same 12 ints, read linearly");
    const int *flat = &m[0][0];
    printf("  ");
    for (size_t i = 0; i < ROWS * COLS; i++) {
        printf("%4d", flat[i]);
    }
    puts("\n  note the rows appear one after another");

    int values[] = { 4, 8, 15, 16, 23, 42 };
    const size_t n = sizeof values / sizeof values[0];

    printf("\nsum      = %d\n", sum(values, n));
    printf("find 23  = index %zu\n", find(values, n, 23));
    printf("find 99  = %s\n", find(values, n, 99) == n ? "not found" : "found");

    reverse(values, n);
    printf("reversed = ");
    for (size_t i = 0; i < n; i++) {
        printf("%d ", values[i]);
    }
    putchar('\n');

    puts("\nbounds-checked access:");
    int got;
    printf("  index 2 -> %s", at(values, n, 2, &got) ? "" : "refused\n");
    if (at(values, n, 2, &got)) printf("%d\n", got);
    printf("  index 99 -> %s\n", at(values, n, 99, &got) ? "returned a value" : "refused");

    return EXIT_SUCCESS;
}
