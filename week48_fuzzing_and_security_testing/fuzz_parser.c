/* fuzz_parser.c */
#include "parser.h"
#include <stdint.h>
#include <stddef.h>

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
{
    if (size > 4096) return 0;              /* keep iterations fast */

    Config *c = config_parse(data, size);
    config_free(c);                          /* free, or the leak checker fires */
    return 0;
}
