/* fake_hal.c — shared by test_protocol.c and fuzz_protocol.c */
#include "fake_hal.h"
#include <string.h>

static bool fake_read(void *ctx, uint8_t *out)
{
    Fake *f = ctx;
    if (f->input[f->input_pos] == '\0') return false;
    *out = (uint8_t)f->input[f->input_pos++];
    return true;
}

static void fake_write(void *ctx, uint8_t byte)
{
    Fake *f = ctx;
    if (f->output_len < sizeof f->output - 1) {
        f->output[f->output_len++] = (char)byte;
        f->output[f->output_len] = '\0';
    }
}

static uint32_t fake_ticks(void *ctx) { return ((Fake *)ctx)->now; }

void fake_init(Fake *f, const char *input, Hal *hal)
{
    memset(f, 0, sizeof *f);
    f->input = input;
    hal->uart_read  = fake_read;
    hal->uart_write = fake_write;
    hal->ticks      = fake_ticks;
    hal->ctx        = f;
}
