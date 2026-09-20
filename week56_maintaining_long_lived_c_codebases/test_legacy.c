/* test_legacy.c */
#include "legacy.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static int checks = 0, failures = 0;

#define CHECK_PARSE(input, want_rc, want_name, want_value) do {        \
    char line[256], name[256] = { 0 };                                 \
    int value = -999;                                                  \
    snprintf(line, sizeof line, "%s", (input));                        \
    int rc = parse_record(line, name, &value);                         \
    checks++;                                                          \
    if (rc != (want_rc) || strcmp(name, (want_name)) != 0              \
        || value != (want_value)) {                                    \
        failures++;                                                    \
        printf("  FAIL \"%s\": rc %d name \"%s\" value %d\n",          \
               (input), rc, name, value);                              \
    }                                                                  \
} while (0)

int main(void)
{
    #include "pinned.h"
    printf("%d checks, %d failed\n", checks, failures);
    return failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
