#include "config.h"

/* Load a configuration file and install it as the active config.
 * On failure the previous configuration remains installed and is
 * unchanged. The caller does not own the result.
 * Returns 0 on success, -1 on failure with errno set where possible. */
int config_reload(const char *path)
{
    int      status = -1;
    FILE    *f      = NULL;
    char    *buf    = NULL;
    Config  *fresh  = NULL;

    f = fopen(path, "r");
    if (f == NULL) {
        goto cleanup;
    }

    buf = malloc(CONFIG_MAX + 1);
    if (buf == NULL) {
        goto cleanup;
    }

    size_t n = fread(buf, 1, CONFIG_MAX, f);
    if (ferror(f)) {
        goto cleanup;
    }
    buf[n] = '\0';                       /* fread does not terminate */

    fresh = calloc(1, sizeof *fresh);    /* calloc: every field defined */
    if (fresh == NULL) {
        goto cleanup;
    }

    char *save = NULL;                   /* strtok_r: no hidden state */
    for (char *line = strtok_r(buf, "\n", &save);
         line != NULL;
         line = strtok_r(NULL, "\n", &save)) {

        char key[32], value[64];
        if (sscanf(line, "%31s = %63s", key, value) != 2) {
            continue;                    /* skip malformed lines */
        }

        if (strcmp(key, "timeout") == 0) {
            long v;
            if (!parse_long(value, &v) || v < 0) {
                goto cleanup;            /* refuse rather than default to 0 */
            }
            fresh->timeout = (int)v;
        } else if (strcmp(key, "name") == 0) {
            snprintf(fresh->name, sizeof fresh->name, "%s", value);
        }
    }

    /* Only now is the old configuration replaced. */
    Config *old = global_config;
    global_config = fresh;
    fresh = NULL;                        /* ownership transferred */
    free(old);
    status = 0;

cleanup:
    free(fresh);                         /* NULL unless we failed */
    free(buf);
    if (f != NULL) {
        fclose(f);
    }
    return status;
}
