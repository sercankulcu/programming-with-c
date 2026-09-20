#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define SAMPLES 100000
#define BUCKETS 10

/* Unbiased 0..n-1, by rejecting the uneven tail of the range. */
static int uniform(int n)
{
    int limit = RAND_MAX - (RAND_MAX % n);
    int value;
    do {
        value = rand();
    } while (value >= limit);
    return value % n;
}

static double mean_of(const double *v, size_t n)
{
    double total = 0.0;
    for (size_t i = 0; i < n; i++) {
        total += v[i];
    }
    return total / (double)n;
}

static double stddev_of(const double *v, size_t n, double mean)
{
    double sum_sq = 0.0;
    for (size_t i = 0; i < n; i++) {
        double d = v[i] - mean;
        sum_sq += d * d;
    }
    return sqrt(sum_sq / (double)n);
}

int main(void)
{
    /* --- calendar time --- */
    puts("== what time is it ==");
    time_t now = time(NULL);
    struct tm local = *localtime(&now);     /* copy: the buffer is static */
    struct tm utc   = *gmtime(&now);

    char stamp[64];
    strftime(stamp, sizeof stamp, "%Y-%m-%d %H:%M:%S", &local);
    printf("  local : %s\n", stamp);
    strftime(stamp, sizeof stamp, "%Y-%m-%d %H:%M:%S UTC", &utc);
    printf("  utc   : %s\n", stamp);
    printf("  epoch : %lld seconds\n", (long long)now);
    printf("  note tm_year is %d (years since 1900) and tm_mon is %d (0-based)\n",
           local.tm_year, local.tm_mon);

    /* --- random numbers --- */
    puts("\n== pseudo-random numbers ==");
    srand((unsigned)now);                   /* seeded ONCE */
    printf("  RAND_MAX = %d\n", RAND_MAX);

    int buckets[BUCKETS] = { 0 };
    double *values = malloc(SAMPLES * sizeof *values);
    if (values == NULL) {
        return EXIT_FAILURE;
    }

    clock_t t0 = clock();
    for (size_t i = 0; i < SAMPLES; i++) {
        int b = uniform(BUCKETS);
        buckets[b]++;
        values[i] = (double)rand() / RAND_MAX;   /* 0.0 .. 1.0 */
    }
    double gen_cpu = (double)(clock() - t0) / CLOCKS_PER_SEC;

    puts("  distribution across 10 buckets (expect about 10000 each):");
    for (int b = 0; b < BUCKETS; b++) {
        printf("    %d: %6d  ", b, buckets[b]);
        for (int bar = 0; bar < buckets[b] / 400; bar++) putchar('#');
        putchar('\n');
    }

    /* --- statistics, using math.h --- */
    puts("\n== statistics ==");
    double m = mean_of(values, SAMPLES);
    double s = stddev_of(values, SAMPLES, m);
    printf("  samples        : %d\n", SAMPLES);
    printf("  mean           : %.6f   (expect 0.5)\n", m);
    printf("  std deviation  : %.6f   (expect 0.2887)\n", s);
    printf("  theoretical sd : %.6f\n", 1.0 / sqrt(12.0));

    /* --- measuring time properly --- */
    puts("\n== cpu time versus wall clock ==");
    printf("  generating %d samples took %.4f CPU seconds\n",
           SAMPLES, gen_cpu);

    t0 = clock();
    time_t wall_start = time(NULL);
    double waste = 0.0;
    for (long i = 0; i < 20000000L; i++) {
        waste += sqrt((double)i);
    }
    double busy_cpu  = (double)(clock() - t0) / CLOCKS_PER_SEC;
    double busy_wall = difftime(time(NULL), wall_start);
    printf("  busy loop: %.4f CPU seconds, %.0f wall seconds (sum %.0f)\n",
           busy_cpu, busy_wall, waste);
    puts("  for a CPU-bound loop these agree; for a sleeping or blocked");
    puts("  program clock() would report almost nothing");

    /* --- math.h traps --- */
    puts("\n== math.h details ==");
    printf("  sqrt(2)       = %.10f\n", sqrt(2.0));
    printf("  pow(2, 10)    = %g\n", pow(2.0, 10.0));
    printf("  2*2*2 by hand = %d   (prefer this for small powers)\n", 2*2*2);
    printf("  fmod(7.5, 2)  = %g   (%% does not work on doubles)\n",
           fmod(7.5, 2.0));
    printf("  round(2.5)    = %g, trunc(2.5) = %g, floor(-2.5) = %g\n",
           round(2.5), trunc(2.5), floor(-2.5));

    errno = 0;
    double bad = sqrt(-1.0);
    printf("  sqrt(-1)      = %f, isnan -> %d, errno==EDOM -> %d\n",
           bad, isnan(bad), errno == EDOM);

    free(values);
    return EXIT_SUCCESS;
}
