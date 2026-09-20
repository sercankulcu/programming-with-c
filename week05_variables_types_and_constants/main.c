#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>
#include <stdbool.h>

int main(void)
{
    printf("CHAR_BIT = %d  (bits per byte on this machine)\n\n", CHAR_BIT);

    printf("%-20s %6s  %s\n", "type", "bytes", "range on this machine");
    printf("%-20s %6zu  %d .. %d\n",
           "char",           sizeof(char),      CHAR_MIN,  CHAR_MAX);
    printf("%-20s %6zu  %d .. %d\n",
           "short",          sizeof(short),     SHRT_MIN,  SHRT_MAX);
    printf("%-20s %6zu  %d .. %d\n",
           "int",            sizeof(int),       INT_MIN,   INT_MAX);
    printf("%-20s %6zu  %ld .. %ld\n",
           "long",           sizeof(long),      LONG_MIN,  LONG_MAX);
    printf("%-20s %6zu  %lld .. %lld\n",
           "long long",      sizeof(long long), LLONG_MIN, LLONG_MAX);
    printf("%-20s %6zu  0 .. %u\n",
           "unsigned int",   sizeof(unsigned),  UINT_MAX);
    printf("%-20s %6zu\n", "float",       sizeof(float));
    printf("%-20s %6zu\n", "double",      sizeof(double));
    printf("%-20s %6zu\n", "long double", sizeof(long double));
    printf("%-20s %6zu\n", "bool",        sizeof(bool));
    printf("%-20s %6zu\n", "size_t",      sizeof(size_t));
    printf("%-20s %6zu\n", "void *",      sizeof(void *));

    printf("\nfloat  : %d significant decimal digits, epsilon = %g\n",
           FLT_DIG, FLT_EPSILON);
    printf("double : %d significant decimal digits, epsilon = %g\n",
           DBL_DIG, DBL_EPSILON);

    puts("\n-- what the standard actually guarantees --");
    puts("char       at least  -127 .. 127");
    puts("short      at least  -32767 .. 32767");
    puts("int        at least  -32767 .. 32767");
    puts("long       at least  -2147483647 .. 2147483647");
    puts("long long  at least  about +/- 9.2e18");

    puts("\n-- the classic surprise --");
    printf("1 / 2       = %d\n",  1 / 2);
    printf("1.0 / 2     = %g\n",  1.0 / 2);
    printf("7 / 2       = %d\n",  7 / 2);
    printf("7 %% 2       = %d\n", 7 % 2);

    return EXIT_SUCCESS;
}
