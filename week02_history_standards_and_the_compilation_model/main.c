#include <stdio.h>

int main(void)
{
    /* A C89-era comment. */
    int total = 0;

    for (int i = 0; i < 5; i++) {   /* declaration inside for: C99 */
        total += i;
    }

    printf("total = %d\n", total);
    return 0;
}
