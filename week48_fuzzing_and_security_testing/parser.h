/* parser.h — the interface the fuzz target drives.
 *
 * Config is opaque: the caller gets a pointer and never sees the layout.
 * That is week 28's technique, applied here so the fuzzer can only reach
 * the code through the same door a real caller would use.
 */
#ifndef PARSER_H
#define PARSER_H

#include <stdint.h>
#include <stddef.h>

typedef struct Config Config;

/* Parse size bytes of configuration text. Returns NULL on allocation
   failure. The result must be released with config_free. */
Config *config_parse(const uint8_t *data, size_t size);

void config_free(Config *c);

#endif /* PARSER_H */
