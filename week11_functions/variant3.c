#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define PASS_MARK 70

/* Each of these does one thing, has a name that says so,
   and can be tested on its own. */

static int sum(const int values[], size_t n)
{
    int total = 0;
    for (size_t i = 0; i < n; i++) {
        total += values[i];
    }
    return total;
}

/* Returns false for an empty array rather than dividing by zero. */
static bool mean(const int values[], size_t n, double *out)
{
    if (n == 0) {
        return false;
    }
    *out = (double)sum(values, n) / (double)n;
    return true;
}

static int maximum(const int values[], size_t n)
{
    int best = values[0];               /* caller guarantees n > 0 */
    for (size_t i = 1; i < n; i++) {
        if (values[i] > best) {
            best = values[i];
        }
    }
    return best;
}

static int minimum(const int values[], size_t n)
{
    int best = values[0];
    for (size_t i = 1; i < n; i++) {
        if (values[i] < best) {
            best = values[i];
        }
    }
    return best;
}

static size_t count_at_least(const int values[], size_t n, int threshold)
{
    size_t found = 0;
    for (size_t i = 0; i < n; i++) {
        if (values[i] >= threshold) {
            found++;
        }
    }
    return found;
}

static void print_report(const int values[], size_t n)
{
    if (n == 0) {
        puts("no scores to report");
        return;
    }

    double average = 0.0;
    mean(values, n, &average);          /* cannot fail: n > 0 checked above */

    printf("%-16s %zu\n",     "count",   n);
    printf("%-16s %.2f\n",    "mean",    average);
    printf("%-16s %d\n",      "highest", maximum(values, n));
    printf("%-16s %d\n",      "lowest",  minimum(values, n));
    printf("%-16s %zu of %zu\n", "passing",
           count_at_least(values, n, PASS_MARK), n);
}

int main(void)
{
    const int scores[] = { 88, 92, 79, 95, 61, 73, 84 };
    const size_t n = sizeof scores / sizeof scores[0];

    print_report(scores, n);

    puts("\n-- the empty case, which the original would have divided by zero --");
    print_report(scores, 0);

    puts("\n-- pass by value --");
    int value = 5;
    printf("before: %d\n", value);
    /* try_to_double(value) would change nothing; see below */
    printf("after:  %d\n", value);

    return EXIT_SUCCESS;
}
