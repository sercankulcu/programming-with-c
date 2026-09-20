#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* Version 1: ranges. An if chain is the natural fit. */
static char grade_from_score(int score)
{
    if (score >= 90) {
        return 'A';
    } else if (score >= 80) {
        return 'B';
    } else if (score >= 70) {
        return 'C';
    } else if (score >= 60) {
        return 'D';
    } else {
        return 'F';
    }
}

/* Version 2: a fixed set of discrete values. switch is the natural fit. */
static const char *describe(char grade)
{
    switch (grade) {
    case 'A':
        return "excellent";
    case 'B':
        return "good";
    case 'C':
        return "satisfactory";
    case 'D':
        return "marginal";
    case 'F':
        return "failing";
    default:
        return "unknown";
    }
}

/* Deliberate fall-through, grouping labels that share one action. */
static bool is_passing(char grade)
{
    switch (grade) {
    case 'A':
    case 'B':
    case 'C':
    case 'D':
        return true;
    default:
        return false;
    }
}

int main(void)
{
    const int scores[] = { 95, 83, 71, 64, 42, 100, 0 };
    const size_t count = sizeof scores / sizeof scores[0];

    printf("%-7s %-7s %-15s %s\n", "score", "grade", "description", "passing");
    for (size_t i = 0; i < count; i++) {
        char g = grade_from_score(scores[i]);
        printf("%-7d %-7c %-15s %s\n",
               scores[i], g, describe(g), is_passing(g) ? "yes" : "no");
    }

    return EXIT_SUCCESS;
}
