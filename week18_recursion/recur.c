#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static long call_counter = 0;

/* --- factorial, to watch frames unwind --- */
static long factorial(int n, int depth)
{
    printf("%*scall  factorial(%d)\n", depth * 2, "", n);
    long result;
    if (n <= 1) {
        result = 1;
    } else {
        result = n * factorial(n - 1, depth + 1);
    }
    printf("%*sreturn %ld\n", depth * 2, "", result);
    return result;
}

/* --- Towers of Hanoi: the definition is itself recursive --- */
static void hanoi(int disks, char from, char to, char via, int *moves)
{
    if (disks == 0) {
        return;
    }
    hanoi(disks - 1, from, via, to, moves);
    (*moves)++;
    if (disks <= 3) {                    /* print only the small cases */
        printf("    move disk %d: %c -> %c\n", disks, from, to);
    }
    hanoi(disks - 1, via, to, from, moves);
}

/* --- binary search, recursive and iterative --- */
static long bsearch_rec(const int a[], long lo, long hi, int target)
{
    if (lo > hi) {
        return -1;                       /* base case: empty range */
    }
    long mid = lo + (hi - lo) / 2;       /* avoids overflow; week 7 */
    if (a[mid] == target) return mid;
    if (a[mid] <  target) return bsearch_rec(a, mid + 1, hi, target);
    return bsearch_rec(a, lo, mid - 1, target);
}

static long bsearch_iter(const int a[], long n, int target)
{
    long lo = 0, hi = n - 1;
    while (lo <= hi) {
        long mid = lo + (hi - lo) / 2;
        if (a[mid] == target) return mid;
        if (a[mid] <  target) lo = mid + 1;
        else                  hi = mid - 1;
    }
    return -1;
}

/* --- Fibonacci three ways --- */
static long fib_naive(int n)
{
    call_counter++;
    if (n <= 1) return n;
    return fib_naive(n - 1) + fib_naive(n - 2);
}

static long fib_memo(int n, long cache[])
{
    call_counter++;
    if (n <= 1)        return n;
    if (cache[n] != 0) return cache[n];
    cache[n] = fib_memo(n - 1, cache) + fib_memo(n - 2, cache);
    return cache[n];
}

static long fib_iter(int n)
{
    long a = 0, b = 1;
    for (int i = 0; i < n; i++) {
        long next = a + b;
        a = b;
        b = next;
    }
    return a;
}

/* --- how deep can we go? --- */
static int max_depth = 0;
static void probe(int depth)
{
    max_depth = depth;
    if (depth < 200000) {
        probe(depth + 1);
    }
}

int main(void)
{
    puts("== frames build up, then unwind ==");
    factorial(4, 0);

    puts("\n== Towers of Hanoi ==");
    for (int disks = 1; disks <= 4; disks++) {
        int moves = 0;
        if (disks <= 3) printf("  %d disks:\n", disks);
        hanoi(disks, 'A', 'C', 'B', &moves);
        printf("  %d disks -> %d moves (2^%d - 1)\n", disks, moves, disks);
    }

    puts("\n== binary search: recursive and iterative agree ==");
    int sorted[] = { 2, 5, 8, 12, 16, 23, 38, 56, 72, 91 };
    const long n = (long)(sizeof sorted / sizeof sorted[0]);
    const int targets[] = { 23, 2, 91, 7 };
    for (size_t i = 0; i < 4; i++) {
        printf("  %2d -> recursive %2ld, iterative %2ld\n",
               targets[i],
               bsearch_rec(sorted, 0, n - 1, targets[i]),
               bsearch_iter(sorted, n, targets[i]));
    }
    printf("  depth for %ld elements is about log2(%ld) = 4\n", n, n);

    puts("\n== the Fibonacci trap ==");
    const int fn = 30;

    call_counter = 0;
    clock_t t0 = clock();
    long r1 = fib_naive(fn);
    double d1 = (double)(clock() - t0) / CLOCKS_PER_SEC;
    printf("  naive  fib(%d) = %ld  in %8.4fs, %ld calls\n",
           fn, r1, d1, call_counter);

    long cache[64];
    memset(cache, 0, sizeof cache);
    call_counter = 0;
    t0 = clock();
    long r2 = fib_memo(fn, cache);
    double d2 = (double)(clock() - t0) / CLOCKS_PER_SEC;
    printf("  memo   fib(%d) = %ld  in %8.4fs, %ld calls\n",
           fn, r2, d2, call_counter);

    t0 = clock();
    long r3 = fib_iter(fn);
    double d3 = (double)(clock() - t0) / CLOCKS_PER_SEC;
    printf("  iter   fib(%d) = %ld  in %8.4fs, 0 recursive calls\n",
           fn, r3, d3);

    puts("\n== how deep before the stack runs out ==");
    puts("  (uncomment the probe call below and watch it die)");
    /* probe(1); printf("reached %d\n", max_depth); */

    return EXIT_SUCCESS;
}
