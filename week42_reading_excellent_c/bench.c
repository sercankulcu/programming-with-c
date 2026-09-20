/* bench.c */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

static size_t naive_strlen(const char *s)
{
    const char *p = s;
    while (*p) p++;
    return (size_t)(p - s);
}

int main(void)
{
    size_t n = 1000000;
    char *big = malloc(n + 1);
    memset(big, 'x', n);
    big[n] = '\0';

    clock_t t0 = clock();
    volatile size_t a = 0;
    for (int i = 0; i < 200; i++) a += naive_strlen(big);
    double naive = (double)(clock() - t0) / CLOCKS_PER_SEC;

    t0 = clock();
    volatile size_t b = 0;
    for (int i = 0; i < 200; i++) b += strlen(big);
    double library = (double)(clock() - t0) / CLOCKS_PER_SEC;

    printf("naive   %.3f s\nlibrary %.3f s\nratio   %.1fx\n",
           naive, library, naive / library);
    free(big);
    return 0;
}
