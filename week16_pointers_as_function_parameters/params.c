#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>

/* Parse a complete decimal integer.
   Returns true and writes *out only on success.
   text is read and never modified, which the signature enforces. */
static bool parse_int(const char *text, int *out)
{
    if (text == NULL || out == NULL) {
        return false;
    }

    while (isspace((unsigned char)*text)) {
        text++;
    }
    if (*text == '\0') {
        return false;                       /* empty or all whitespace */
    }

    errno = 0;
    char *end = NULL;                       /* strtol writes here */
    long value = strtol(text, &end, 10);

    if (end == text)      return false;     /* no digits at all      */
    if (*end != '\0')     return false;     /* trailing rubbish      */
    if (errno == ERANGE)  return false;     /* out of long's range   */
    if (value < INT_MIN || value > INT_MAX) return false;

    *out = (int)value;
    return true;
}

/* Two results, so two output parameters — and at three it would be a struct. */
static bool min_max(const int *values, size_t n, int *lo, int *hi)
{
    if (values == NULL || n == 0) {
        return false;
    }
    *lo = *hi = values[0];
    for (size_t i = 1; i < n; i++) {
        if (values[i] < *lo) *lo = values[i];
        if (values[i] > *hi) *hi = values[i];
    }
    return true;
}

/* Caller supplies the buffer and its size: cannot leak, cannot overflow. */
static bool describe_into(char *dst, size_t dst_size, int lo, int hi)
{
    int n = snprintf(dst, dst_size, "range %d..%d (span %d)", lo, hi, hi - lo);
    return n >= 0 && (size_t)n < dst_size;
}

/* Safe: the literals have static storage duration. */
static const char *verdict(int span)
{
    if (span == 0)  return "all equal";
    if (span < 10)  return "tight";
    if (span < 100) return "moderate";
    return "wide";
}

/* const correctness: this reads, so every pointer it takes is const. */
static size_t count_matching(const int *values, size_t n, int target)
{
    size_t found = 0;
    for (size_t i = 0; i < n; i++) {
        if (values[i] == target) {
            found++;
        }
    }
    return found;
}

int main(void)
{
    puts("== parsing, with failure reported rather than guessed ==");
    const char *inputs[] = { "42", "  -7 ", "abc", "", "12abc", "99999999999" };
    const size_t input_count = sizeof inputs / sizeof inputs[0];

    int parsed[8];
    size_t good = 0;

    for (size_t i = 0; i < input_count; i++) {
        int value;
        if (parse_int(inputs[i], &value)) {
            printf("  \"%s\" -> %d\n", inputs[i], value);
            parsed[good++] = value;
        } else {
            printf("  \"%s\" -> rejected\n", inputs[i]);
        }
    }

    puts("\n== two outputs ==");
    int lo, hi;
    if (min_max(parsed, good, &lo, &hi)) {
        printf("  lowest %d, highest %d\n", lo, hi);

        char text[64];
        if (describe_into(text, sizeof text, lo, hi)) {
            printf("  %s — %s\n", text, verdict(hi - lo));
        }

        char tiny[8];
        if (!describe_into(tiny, sizeof tiny, lo, hi)) {
            printf("  into an 8-byte buffer: truncated to \"%s\", and detected\n",
                   tiny);
        }
    }

    puts("\n== the empty case is a failure, not a wrong answer ==");
    printf("  min_max on 0 values -> %s\n",
           min_max(parsed, 0, &lo, &hi) ? "succeeded?!" : "correctly refused");

    puts("\n== const correctness ==");
    printf("  count of 42 = %zu\n", count_matching(parsed, good, 42));
    puts("  count_matching takes const int * — the compiler guarantees");
    puts("  it cannot modify the caller's array, and says so in the signature");

    puts("\n== declarations, read with the right-left rule ==");
    int   n1 = 5;
    int  *p1 = &n1;             /* p1: pointer to int                    */
    int  *a1[3] = { &n1, NULL, NULL };  /* a1: array of 3 pointers to int */
    int   m[2][3] = { { 1, 2, 3 }, { 4, 5, 6 } };
    int (*r1)[3] = m;           /* r1: pointer to array of 3 int         */

    printf("  *p1      = %d\n", *p1);
    printf("  *a1[0]   = %d\n", *a1[0]);
    printf("  r1[1][2] = %d\n", r1[1][2]);
    printf("  sizeof a1 = %zu (3 pointers), sizeof *r1 = %zu (3 ints)\n",
           sizeof a1, sizeof *r1);

    return EXIT_SUCCESS;
}
