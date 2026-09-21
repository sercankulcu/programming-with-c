/* test_parser.c - the same pinned assertions, driving the new parser.
 *
 * pinned.h is not regenerated. It still records what the legacy code did.
 * Only the macro body changes, because parse_record now takes the size of
 * the destination and returns bool. If these assertions still pass, the
 * rewrite changed no behavior - which is the whole claim being made.
 */
#include "parser.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static int checks = 0, failures = 0;

#define CHECK_PARSE(input, want_rc, want_name, want_value) do {         \
    char line[256], name[256] = { 0 };                                  \
    int value = -999;                                                   \
    snprintf(line, sizeof line, "%s", (input));                         \
    int rc = parse_record(line, name, sizeof name, &value) ? 1 : 0;     \
    checks++;                                                           \
    if (rc != (want_rc) || strcmp(name, (want_name)) != 0               \
        || value != (want_value)) {                                     \
        failures++;                                                     \
        printf("  FAIL \"%s\": rc %d name \"%s\" value %d\n",           \
               (input), rc, name, value);                               \
    }                                                                   \
} while (0)

int main(void)
{
    #include "pinned.h"
    printf("%d checks, %d failed\n", checks, failures);
    return failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
