/* parser.c — after the refactoring, before the behavior fixes */
#include "parser.h"
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

#define LINE_MAX_LEN 256

static const char *skip_space(const char *s)
{
    while (*s == ' ' || *s == '\t') s++;
    return s;
}

static void trim_trailing(char *s)
{
    size_t n = strlen(s);
    while (n > 0 && (s[n-1] == ' ' || s[n-1] == '\t')) s[--n] = '\0';
}

static bool parse_long(const char *text, long *out)
{
    if (text == NULL || *text == '\0') return false;
    errno = 0;
    char *end;
    long v = strtol(text, &end, 10);
    if (end == text || errno == ERANGE) return false;
    *out = v;
    return true;
}

/* No globals: safe to call from two threads. */
bool parse_record(const char *line, char *out_name, size_t name_size,
                  int *out_value)
{
    if (line == NULL || out_name == NULL || out_value == NULL) {
        return false;
    }

    char work[LINE_MAX_LEN];
    if ((size_t)snprintf(work, sizeof work, "%s", line) >= sizeof work) {
        return false;                       /* too long: refuse, not truncate */
    }

    char *equals = strchr(work, '=');
    if (equals == NULL) {
        return false;
    }
    *equals = '\0';

    const char *name = skip_space(work);
    trim_trailing((char *)name);

    if ((size_t)snprintf(out_name, name_size, "%s", name) >= name_size) {
        return false;                       /* caller's buffer too small */
    }

    const char *value_text = skip_space(equals + 1);

    long v;
    /* TODO(#412): pinned legacy behavior — a malformed value yields 0.
       Changing this to a rejection is a separate, reviewed change. */
    *out_value = (parse_long(value_text, &v) && v >= INT_MIN && v <= INT_MAX)
               ? (int)v : 0;
    return true;
}
