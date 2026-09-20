#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    const int rows = 5;
    const int columns = 3;

    const int cell_count = rows * columns;
    printf("cells in the grid: %d\n", cell_count);

    /* Sum 1..cell_count, used later to size the report buffer. */
    int running_total = 0;
    for (int i = 1; i <= cell_count; i++) {
        running_total += i;
    }
    printf("triangular number for %d: %d\n", cell_count, running_total);

    return EXIT_SUCCESS;
}
