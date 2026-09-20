#include <stdio.h>
#include <stdlib.h>

static int trace(const char *label, int value)
{
    printf("  [evaluating %s]\n", label);
    return value;
}

int main(void)
{
    puts("== integer division and remainder ==");
    printf("7 / 2        = %d\n",  7 / 2);
    printf("-7 / 2       = %d\n",  -7 / 2);
    printf("7 %% 2        = %d\n", 7 % 2);
    printf("-7 %% 2       = %d\n", -7 % 2);
    printf("7.0 / 2      = %g\n",  7.0 / 2);

    puts("\n== precedence ==");
    printf("2 + 3 * 4        = %d\n", 2 + 3 * 4);
    printf("(2 + 3) * 4      = %d\n", (2 + 3) * 4);
    printf("10 - 4 - 3       = %d\n", 10 - 4 - 3);
    printf("1 << 2 + 3       = %d\n", 1 << 2 + 3);   /* 1 << (2+3) */
    printf("(1 << 2) + 3     = %d\n", (1 << 2) + 3);

    puts("\n== the bitwise-versus-comparison trap ==");
    int flags = 0x0C;          /* 0000 1100 */
    int mask  = 0x04;          /* 0000 0100 */
    printf("flags & mask == 0    -> %d   (this is flags & (mask == 0))\n",
           flags & mask == 0);
    printf("(flags & mask) == 0  -> %d   (what you meant)\n",
           (flags & mask) == 0);

    puts("\n== pre versus post increment ==");
    int i = 5;
    printf("i++ yields %d, i is now %d\n", i++, i);
    int j = 5;
    printf("++j yields %d, j is now %d\n", ++j, j);

    puts("\n== short-circuit is guaranteed ==");
    int divisor = 0;
    if (divisor != 0 && 100 / divisor > 10) {
        puts("  not reached");
    }
    puts("  survived a division by zero that never happened");

    puts("\n== argument order is NOT guaranteed ==");
    int sum = trace("left", 1) + trace("right", 2);
    printf("  sum = %d\n", sum);
    printf("  rerun with -O2 and the two lines may swap\n");

    return EXIT_SUCCESS;
}
