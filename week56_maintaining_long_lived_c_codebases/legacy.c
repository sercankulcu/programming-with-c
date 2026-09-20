/* legacy.c — as found */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char buf[256];
int count;

parse_record(line, out_name, out_value)
char *line;
char *out_name;
int *out_value;
{
    char *p, *q;
    int i;

    strcpy(buf, line);

    p = buf;
    while (*p == ' ' || *p == '\t') p++;

    q = p;
    while (*q != '=' && *q != 0) q++;
    if (*q == 0) return 0;

    *q = 0;
    i = strlen(p);
    while (i > 0 && (p[i-1] == ' ' || p[i-1] == '\t')) { p[i-1] = 0; i--; }

    strcpy(out_name, p);

    q++;
    while (*q == ' ' || *q == '\t') q++;
    *out_value = atoi(q);

    count++;
    return 1;
}
