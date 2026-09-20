#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* Throw away the remainder of the current input line.
   Returns false if input ended while doing so. */
static bool discard_line(void)
{
    int c;
    while ((c = getchar()) != '\n') {
        if (c == EOF) {
            return false;
        }
    }
    return true;
}

/* Read one integer in [low, high].
   Returns false only if input ends; otherwise keeps asking. */
static bool read_int_in_range(const char *prompt, int low, int high, int *out)
{
    for (;;) {
        printf("%s (%d-%d): ", prompt, low, high);
        fflush(stdout);

        int value;
        int converted = scanf("%d", &value);

        if (converted == EOF) {
            putchar('\n');
            return false;                    /* Ctrl-D, or piped input ran out */
        }

        if (converted == 0) {
            fprintf(stderr, "  not a number — try again\n");
            if (!discard_line()) {           /* essential: remove the bad text */
                return false;
            }
            continue;
        }

        if (!discard_line()) {               /* remove the trailing newline
                                                and anything after the number */
            return false;
        }

        if (value < low || value > high) {
            fprintf(stderr, "  %d is out of range — try again\n", value);
            continue;
        }

        *out = value;
        return true;
    }
}

int main(void)
{
    int score;
    if (!read_int_in_range("Enter a score", 1, 100, &score)) {
        fprintf(stderr, "no input; giving up\n");
        return EXIT_FAILURE;
    }

    printf("\n%-12s %s\n", "score", "bar");
    printf("%-12d ", score);
    for (int i = 0; i < score / 5; i++) {
        putchar('#');
    }
    printf("\n%-12s %6.1f%%\n", "as percent", (double)score);

    return EXIT_SUCCESS;
}
