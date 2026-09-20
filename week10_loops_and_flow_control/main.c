#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define LIMIT 50
#define TABLE 9

/* Sieve of Eratosthenes over [2, LIMIT].
   Invariant: after processing p, every composite with a factor <= p
   has been marked false. */
static void print_primes(void)
{
    bool is_prime[LIMIT + 1];

    for (size_t i = 0; i <= LIMIT; i++) {
        is_prime[i] = true;
    }
    is_prime[0] = false;
    is_prime[1] = false;

    for (size_t p = 2; p * p <= LIMIT; p++) {
        if (!is_prime[p]) {
            continue;                 /* already crossed out */
        }
        /* Start at p*p: smaller multiples of p have a smaller
           factor and were crossed out in an earlier pass. */
        for (size_t multiple = p * p; multiple <= LIMIT; multiple += p) {
            is_prime[multiple] = false;
        }
    }

    printf("primes up to %d:\n ", LIMIT);
    size_t printed = 0;
    for (size_t i = 2; i <= LIMIT; i++) {
        if (is_prime[i]) {
            printf("%4zu", i);
            if (++printed % 10 == 0) {
                printf("\n ");
            }
        }
    }
    putchar('\n');
}

/* A multiplication table with aligned headers. */
static void print_table(void)
{
    printf("\nmultiplication table\n");

    printf("    ");
    for (size_t col = 1; col <= TABLE; col++) {
        printf("%4zu", col);
    }
    printf("\n    ");
    for (size_t col = 1; col <= TABLE; col++) {
        printf("----");
    }
    putchar('\n');

    for (size_t row = 1; row <= TABLE; row++) {
        printf("%2zu |", row);
        for (size_t col = 1; col <= TABLE; col++) {
            printf("%4zu", row * col);
        }
        putchar('\n');
    }
}

/* Linear search returning the count as "not found",
   so the caller needs no separate flag. */
static size_t find(const int values[], size_t n, int target)
{
    for (size_t i = 0; i < n; i++) {
        if (values[i] == target) {
            return i;
        }
    }
    return n;
}

int main(void)
{
    print_primes();
    print_table();

    const int data[] = { 4, 8, 15, 16, 23, 42 };
    const size_t n = sizeof data / sizeof data[0];

    printf("\nsearching %zu values\n", n);
    const int targets[] = { 15, 99 };
    for (size_t t = 0; t < 2; t++) {
        size_t at = find(data, n, targets[t]);
        if (at == n) {
            printf("  %d: not found\n", targets[t]);
        } else {
            printf("  %d: at index %zu\n", targets[t], at);
        }
    }

    /* The boundary case every search must survive. */
    printf("  empty array: %s\n",
           find(data, 0, 15) == 0 ? "correctly reports not found" : "BUG");

    return EXIT_SUCCESS;
}
