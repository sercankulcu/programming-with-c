/* fake_hal.h — a scripted hardware layer for host builds */
#ifndef FAKE_HAL_H
#define FAKE_HAL_H
#include "hal.h"
#include <stddef.h>

/* Input from a string, output captured, time set by the test. */
typedef struct {
    const char *input;
    size_t      input_pos;
    char        output[1024];
    size_t      output_len;
    uint32_t    now;                 /* time we control exactly */
} Fake;

void fake_init(Fake *f, const char *input, Hal *hal);

#endif
