/* parser.c — with three planted defects */
#include "parser.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#define MAX_NAME 32

struct Config {
    int    timeout;
    int    retries;
    char   name[MAX_NAME];
    int   *values;
    size_t value_count;
};

Config *config_parse(const uint8_t *data, size_t size)
{
    Config *c = calloc(1, sizeof *c);
    if (c == NULL) return NULL;

    char *text = malloc(size + 1);
    if (text == NULL) { free(c); return NULL; }
    memcpy(text, data, size);
    text[size] = '\0';

    char *save = NULL;
    for (char *line = strtok_r(text, "\n", &save);
         line != NULL;
         line = strtok_r(NULL, "\n", &save)) {

        if (line[0] == '#' || line[0] == '\0') continue;

        char *eq = strchr(line, '=');
        if (eq == NULL) continue;
        *eq = '\0';
        char *key   = line;
        char *value = eq + 1;

        while (*key == ' ')   key++;
        while (*value == ' ') value++;

        if (strcmp(key, "timeout") == 0) {
            c->timeout = atoi(value);                 /* DEFECT 1 */
        } else if (strcmp(key, "name") == 0) {
            strcpy(c->name, value);                   /* DEFECT 2 */
        } else if (strcmp(key, "values") == 0) {
            size_t n = (size_t)atoi(value);
            c->values = malloc(n * sizeof *c->values); /* DEFECT 3 */
            c->value_count = n;
            for (size_t i = 0; i < n; i++) c->values[i] = 0;
        }
    }

    free(text);
    return c;
}

void config_free(Config *c)
{
    if (c == NULL) return;
    free(c->values);
    free(c);
}
