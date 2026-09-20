/* characterize.c — generates the assertions */
#include <stdio.h>
#include <string.h>

int parse_record();                 /* no prototype exists yet */

int main(void)
{
    const char *inputs[] = {
        "timeout = 30", "  name = 42  ", "key=7", "noequals",
        "= 5", "empty =", "x = abc", "x = 99999999999", ""
    };

    for (size_t i = 0; i < sizeof inputs / sizeof inputs[0]; i++) {
        char name[256] = { 0 };
        int  value = -999;
        char copy[256];
        snprintf(copy, sizeof copy, "%s", inputs[i]);

        int rc = parse_record(copy, name, &value);
        printf("CHECK_PARSE(\"%s\", %d, \"%s\", %d);\n",
               inputs[i], rc, name, value);
    }
    return 0;
}
