/* hal.h */
#ifndef HAL_H
#define HAL_H
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    bool     (*uart_read)(void *ctx, uint8_t *out);
    void     (*uart_write)(void *ctx, uint8_t byte);
    uint32_t (*ticks)(void *ctx);
    void     *ctx;
} Hal;
#endif
