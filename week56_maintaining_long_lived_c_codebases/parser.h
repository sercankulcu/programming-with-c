/* parser.h - the modernized parser from step 4.
 *
 * legacy.c is the 1990s original; this is the same behavior behind a
 * prototype the compiler can check.
 */
#ifndef PARSER_H
#define PARSER_H

#include <stddef.h>
#include <stdbool.h>

/* Parse "name,value" out of line. Writes at most name_size bytes to out_name
   (always terminated) and the number to out_value. Returns false and leaves
   the outputs untouched if the line does not parse. */
bool parse_record(const char *line, char *out_name, size_t name_size,
                  long *out_value);

#endif /* PARSER_H */
