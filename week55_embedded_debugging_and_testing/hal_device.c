/* hal_device.c — only this file knows about registers */
#include "hal.h"

#define UART0_DR (*(volatile uint32_t *)0x4000C000u)
#define UART0_FR (*(volatile uint32_t *)0x4000C018u)
#define SYST_CVR (*(volatile uint32_t *)0xE000E018u)

static bool dev_read(void *ctx, uint8_t *out)
{
    (void)ctx;
    if (UART0_FR & (1u << 4)) return false;      /* RXFE: empty */
    *out = (uint8_t)(UART0_DR & 0xFFu);
    return true;
}

static void dev_write(void *ctx, uint8_t byte)
{
    (void)ctx;
    while (UART0_FR & (1u << 5)) { }            /* TXFF: full */
    UART0_DR = byte;
}

static uint32_t dev_ticks(void *ctx) { (void)ctx; return SYST_CVR; }

const Hal device_hal = {
    .uart_read  = dev_read,
    .uart_write = dev_write,
    .ticks      = dev_ticks,
    .ctx        = NULL
};
