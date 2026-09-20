#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SIZE 1024
#define REPS 20
#define RECORDS 2000000

static double now(void)
{
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return (double)t.tv_sec + (double)t.tv_nsec / 1e9;
}

/* ---------- 1. loop order over a matrix ---------- */

static double sum_row_major(const double *m)
{
    double total = 0.0;
    for (int rep = 0; rep < REPS; rep++)
        for (size_t r = 0; r < SIZE; r++)
            for (size_t c = 0; c < SIZE; c++)
                total += m[r * SIZE + c];         /* sequential */
    return total;
}

static double sum_column_major(const double *m)
{
    double total = 0.0;
    for (int rep = 0; rep < REPS; rep++)
        for (size_t c = 0; c < SIZE; c++)
            for (size_t r = 0; r < SIZE; r++)
                total += m[r * SIZE + c];         /* strides 8 KB per step */
    return total;
}

/* ---------- 2. array of structures versus structure of arrays ---------- */

typedef struct {
    int    id;
    char   name[32];
    double score;
} Record;                       /* 48 bytes; we want 8 of them */

typedef struct {
    int    *id;
    char  (*name)[32];
    double *score;
} Columns;

static double sum_aos(const Record *r, size_t n)
{
    double total = 0.0;
    for (int rep = 0; rep < 20; rep++)
        for (size_t i = 0; i < n; i++)
            total += r[i].score;
    return total;
}

static double sum_soa(const Columns *c, size_t n)
{
    double total = 0.0;
    for (int rep = 0; rep < 20; rep++)
        for (size_t i = 0; i < n; i++)
            total += c->score[i];
    return total;
}

/* ---------- 3. what the optimizer can and cannot see ---------- */

static long sum_to(long n)
{
    long total = 0;
    for (long i = 1; i <= n; i++) total += i;
    return total;
}

int main(void)
{
    puts("== 1. loop order ==");
    double *m = malloc((size_t)SIZE * SIZE * sizeof *m);
    if (m == NULL) return EXIT_FAILURE;
    for (size_t i = 0; i < (size_t)SIZE * SIZE; i++) m[i] = 1.0;
    printf("  matrix is %zu MB, cache lines hold %zu doubles\n",
           (size_t)SIZE * SIZE * sizeof *m / (1024 * 1024), 64 / sizeof(double));

    double t = now();
    double a = sum_row_major(m);
    double row_secs = now() - t;

    t = now();
    double b = sum_column_major(m);
    double col_secs = now() - t;

    printf("  row-major    : %.4f s  (sum %.0f)\n", row_secs, a);
    printf("  column-major : %.4f s  (sum %.0f)\n", col_secs, b);
    printf("  same result, same operation count, %.1fx difference\n",
           col_secs / row_secs);
    free(m);

    puts("\n== 2. array of structures versus structure of arrays ==");
    Record *recs = malloc(RECORDS * sizeof *recs);
    Columns cols = {
        .id    = malloc(RECORDS * sizeof *cols.id),
        .name  = malloc(RECORDS * sizeof *cols.name),
        .score = malloc(RECORDS * sizeof *cols.score)
    };
    if (recs == NULL || cols.id == NULL || cols.name == NULL || cols.score == NULL)
        return EXIT_FAILURE;

    for (size_t i = 0; i < RECORDS; i++) {
        recs[i].id = (int)i;  recs[i].score = 1.0;
        cols.id[i] = (int)i;  cols.score[i] = 1.0;
    }

    t = now();  double s1 = sum_aos(recs, RECORDS);  double aos_secs = now() - t;
    t = now();  double s2 = sum_soa(&cols, RECORDS); double soa_secs = now() - t;

    printf("  sizeof(Record) = %zu, so a 64-byte line holds %.1f scores\n",
           sizeof(Record), 64.0 / (double)sizeof(Record));
    printf("  array of structures : %.4f s  (sum %.0f)\n", aos_secs, s1);
    printf("  structure of arrays : %.4f s  (sum %.0f)\n", soa_secs, s2);
    printf("  %.1fx, purely from how much of each line is useful\n",
           aos_secs / soa_secs);

    free(recs); free(cols.id); free(cols.name); free(cols.score);

    puts("\n== 3. what the optimizer does ==");
    t = now();
    long total = sum_to(100000000L);
    double loop_secs = now() - t;
    printf("  sum 1..100000000 = %ld in %.4f s\n", total, loop_secs);
    puts("  compile at -O2 and inspect the assembly:");
    puts("    gcc -O2 -S perf.c -o perf.s && grep -A20 'sum_to:' perf.s");
    puts("  the loop may have become a closed-form multiplication");

    return EXIT_SUCCESS;
}
