#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include <limits.h>

static bool nearly_equal(double a, double b, double relative)
{
    double diff = fabs(a - b);
    if (diff <= DBL_EPSILON) {
        return true;
    }
    double scale = fmax(fabs(a), fabs(b));
    return diff <= relative * scale;
}

int main(void)
{
    puts("== bug 1: a comparison that is wrong but not incorrect ==");
    int      signed_value   = -1;
    unsigned unsigned_value = 1;
    printf("  -1 < 1u  evaluates to %d\n", signed_value < unsigned_value);
    printf("  because -1 converted to unsigned is %u\n",
           (unsigned)signed_value);
    printf("  fix: compare in a signed type -> %d\n",
           signed_value < (int)unsigned_value);

    puts("\n== the loop that would never end ==");
    const char *empty = "";
    size_t length = strlen(empty);
    printf("  strlen(\"\") = %zu\n", length);
    printf("  length - 1 = %zu   <-- wrapped\n", length - 1);
    puts("  so  for (i = 0; i < length - 1; i++)  would run ~1.8e19 times");
    puts("  fix: write  i + 1 < length  instead");

    puts("\n== bug 2: floating point equality ==");
    double a = 0.1, b = 0.2;
    printf("  0.1 + 0.2        = %.20f\n", a + b);
    printf("  == 0.3           -> %s\n", (a + b == 0.3) ? "true" : "false");
    printf("  nearly_equal     -> %s\n",
           nearly_equal(a + b, 0.3, 1e-9) ? "true" : "false");

    double sum = 0.0;
    for (int i = 0; i < 10; i++) {
        sum += 0.1;
    }
    printf("  0.1 added ten times = %.20f\n", sum);
    printf("  == 1.0           -> %s\n", (sum == 1.0) ? "true" : "false");
    printf("  nearly_equal     -> %s\n",
           nearly_equal(sum, 1.0, 1e-9) ? "true" : "false");

    puts("\n== promotion and narrowing ==");
    char small_a = 100, small_b = 100;
    printf("  100 + 100 as int  = %d\n", small_a + small_b);
    printf("  stored in a char  = %d   <-- did not fit\n",
           (char)(small_a + small_b));

    printf("\n  (int)3.99  = %d      (truncates, never rounds)\n", (int)3.99);
    printf("  (int)-3.99 = %d\n", (int)-3.99);
    printf("  lround(3.99) = %ld\n", lround(3.99));

    puts("\n== the average, done wrong and right ==");
    int total = 7, count = 2;
    printf("  total / count             = %d\n", total / count);
    printf("  (double)(total / count)   = %g   <-- cast too late\n",
           (double)(total / count));
    printf("  (double)total / count     = %g   <-- correct\n",
           (double)total / count);

    puts("\n== NaN and infinity ==");
    double nan_value = 0.0 / 0.0;
    double inf_value = 1.0 / 0.0;
    printf("  0.0/0.0 = %f, isnan -> %d, equals itself -> %d\n",
           nan_value, isnan(nan_value), nan_value == nan_value);
    printf("  1.0/0.0 = %f, isinf -> %d\n", inf_value, isinf(inf_value));

    return EXIT_SUCCESS;
}
