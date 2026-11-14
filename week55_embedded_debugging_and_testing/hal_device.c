/* hal_device.c — only this file knows about registers */
#include "hal.h"
#include <stddef.h>

#define UART0_DR (*(volatile uint32_t *)0x4000C000u)
#define UART0_FR (*(volatile uint32_t *)0x4000C018u)

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

/* Counted up by the SysTick interrupt (week 51). SYST_CVR itself counts
   down and reloads, so now - last would be meaningless across a reload. */
static volatile uint32_t tick_count;

void SysTick_Handler(void) { tick_count++; }

static uint32_t dev_ticks(void *ctx) { (void)ctx; return tick_count; }

const Hal device_hal = {
    .uart_read  = dev_read,
    .uart_write = dev_write,
    .ticks      = dev_ticks,
    .ctx        = NULL
};
