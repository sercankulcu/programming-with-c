/* config.h - just enough context to compile the function under review.
 *
 * This week is about reading a change, not running a program. The reviewed
 * function is config_reload in config.c; everything here exists so that it
 * can be compiled on its own and the compiler's opinion counted as part of
 * the review.
 */
#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define CONFIG_MAX 8191

typedef struct Config {
    int  timeout;
    char name[64];
} Config;

/* The currently installed configuration. Defined elsewhere in the program. */
extern Config *global_config;

/* Week 23's checked conversion. */
bool parse_long(const char *text, long *out);

int config_reload(const char *path);

#endif /* CONFIG_H */
