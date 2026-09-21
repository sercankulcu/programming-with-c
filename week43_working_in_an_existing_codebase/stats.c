#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Returns the mean of n values. Caller guarantees n > 0. */
static double mean(const int *v, size_t n)
{
    long total = 0;
    for (size_t i = 0; i < n; i++) total += v[i];
    return (double)total / (double)n;
}

static int maximum(const int *v, size_t n)
{
    int best = v[0];
    for (size_t i = 1; i < n; i++) if (v[i] > best) best = v[i];
    return best;
}

int main(int argc, char *argv[])
{
    if (argc < 2) { fprintf(stderr, "usage: %s N...\n", argv[0]); return 2; }

    size_t n = (size_t)(argc - 1);
    int *v = malloc(n * sizeof *v);
    if (v == NULL) return 1;
    for (size_t i = 0; i < n; i++) v[i] = atoi(argv[i + 1]);

    printf("count %zu mean %.2f max %d\n", n, mean(v, n), maximum(v, n));
    free(v);
    return 0;
}
