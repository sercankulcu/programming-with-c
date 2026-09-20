#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <limits.h>

static const char *program_name = "scale";

static void usage(void)
{
    fprintf(stderr,
        "usage: %s SCALE OFFSET\n"
        "  reads one number per line from standard input,\n"
        "  prints value * SCALE + OFFSET to standard output\n",
        program_name);
}

/* Full validation: empty, non-numeric, trailing text, out of range. */
static bool parse_double(const char *text, double *out)
{
    if (text == NULL || *text == '\0') {
        return false;
    }
    errno = 0;
    char *end;
    double value = strtod(text, &end);

    if (end == text)     return false;      /* no conversion   */
    while (*end == ' ' || *end == '\t') end++;   /* allow trailing spaces */
    if (*end != '\0')    return false;      /* trailing rubbish */
    if (errno == ERANGE) return false;      /* overflow/underflow */

    *out = value;
    return true;
}

static bool parse_int(const char *text, int *out)
{
    if (text == NULL || *text == '\0') {
        return false;
    }
    errno = 0;
    char *end;
    long value = strtol(text, &end, 10);

    if (end == text)                        return false;
    if (*end != '\0')                       return false;
    if (errno == ERANGE)                    return false;
    if (value < INT_MIN || value > INT_MAX) return false;

    *out = (int)value;
    return true;
}

int main(int argc, char *argv[])
{
    if (argc > 0 && argv[0] != NULL) {
        program_name = argv[0];
    }

    if (argc != 3) {
        fprintf(stderr, "%s: expected 2 arguments, got %d\n",
                program_name, argc - 1);
        usage();
        return EXIT_FAILURE;
    }

    double scale, offset;
    if (!parse_double(argv[1], &scale)) {
        fprintf(stderr, "%s: SCALE is not a number: \"%s\"\n",
                program_name, argv[1]);
        return EXIT_FAILURE;
    }
    if (!parse_double(argv[2], &offset)) {
        fprintf(stderr, "%s: OFFSET is not a number: \"%s\"\n",
                program_name, argv[2]);
        return EXIT_FAILURE;
    }

    char line[256];
    long line_number = 0;
    long accepted = 0, rejected = 0;

    while (fgets(line, sizeof line, stdin) != NULL) {
        line_number++;

        /* Detect a line too long for the buffer. */
        if (strchr(line, '\n') == NULL && !feof(stdin)) {
            fprintf(stderr, "%s: line %ld too long, skipping\n",
                    program_name, line_number);
            int c;
            while ((c = getchar()) != '\n' && c != EOF) { }
            rejected++;
            continue;
        }

        line[strcspn(line, "\n")] = '\0';

        if (line[0] == '\0' || line[0] == '#') {
            continue;                        /* blank line or comment */
        }

        double value;
        if (!parse_double(line, &value)) {
            fprintf(stderr, "%s: line %ld: not a number: \"%s\"\n",
                    program_name, line_number, line);
            rejected++;
            continue;
        }

        printf("%g\n", value * scale + offset);   /* result to stdout */
        accepted++;
    }

    if (ferror(stdin)) {
        fprintf(stderr, "%s: error reading input\n", program_name);
        return EXIT_FAILURE;
    }

    fprintf(stderr, "%s: %ld accepted, %ld rejected\n",
            program_name, accepted, rejected);

    return rejected == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
