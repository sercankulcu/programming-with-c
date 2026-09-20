/* fuzz_protocol.c */
#include "protocol.h"
#include <string.h>

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
{
    if (size > 4096) return 0;
    char *input = malloc(size + 1);
    memcpy(input, data, size);
    input[size] = '\0';

    Fake f; Hal hal; Protocol p;
    fake_init(&f, input, &hal);
    protocol_init(&p, &hal);
    for (int i = 0; i < 4; i++) protocol_poll(&p);

    free(input);
    return 0;
}
