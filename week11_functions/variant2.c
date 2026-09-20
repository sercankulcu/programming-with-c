#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int scores[] = { 88, 92, 79, 95, 61, 73, 84 };
    size_t n = sizeof scores / sizeof scores[0];

    int total = 0;
    for (size_t i = 0; i < n; i++) total += scores[i];
    double mean = (double)total / n;

    int hi = scores[0], lo = scores[0];
    for (size_t i = 1; i < n; i++) {
        if (scores[i] > hi) hi = scores[i];
        if (scores[i] < lo) lo = scores[i];
    }

    int passing = 0;
    for (size_t i = 0; i < n; i++) if (scores[i] >= 70) passing++;

    printf("n=%zu mean=%.2f hi=%d lo=%d pass=%d\n", n, mean, hi, lo, passing);
    return 0;
}
