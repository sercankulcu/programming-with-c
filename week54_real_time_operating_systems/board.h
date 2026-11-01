/* board.h - the little the two schedulers need from the board.
 *
 * Same lm3s6965 UART as weeks 49-51. Everything is inline except
 * tick_count (board.c) and the superloop's SysTick handler (tick.c).
 */
#ifndef BOARD_H
#define BOARD_H

#include <stdint.h>

#define UART0_DR (*(volatile uint32_t *)0x4000C000u)

/* SysTick, in the core peripheral region (week 51) */
#define SYST_CSR (*(volatile uint32_t *)0xE000E010u)
#define SYST_RVR (*(volatile uint32_t *)0xE000E014u)
#define SYST_CVR (*(volatile uint32_t *)0xE000E018u)

/* QEMU's lm3s6965evb comes out of reset at 12.5 MHz (200 MHz / 16).
   FreeRTOSConfig.h repeats this as configCPU_CLOCK_HZ. */
#define BOARD_CPU_HZ  12500000u
#define BOARD_TICK_HZ 1000u             /* one tick = 1 ms, as in the notes */

extern volatile uint32_t tick_count;

static inline void board_init(void)
{
    tick_count = 0;
#ifndef INC_FREERTOS_H
    /* The superloop counts its own ticks. Under FreeRTOS the kernel owns
       SysTick and starts it in vTaskStartScheduler; starting it here too
       would deliver ticks to a scheduler that is not running yet. */
    SYST_RVR = BOARD_CPU_HZ / BOARD_TICK_HZ - 1u;
    SYST_CVR = 0;
    SYST_CSR = 0x7u;                    /* enable, interrupt, processor clock */
#endif
}

static inline void uart_puts(const char *s)
{
    for (; *s; s++) { UART0_DR = (uint32_t)(unsigned char)*s; }
}

static inline void put_int(uint32_t v)
{
    char buf[11];
    int n = 0;
    if (v == 0) { UART0_DR = (uint32_t)'0'; return; }
    while (v) { buf[n++] = (char)('0' + v % 10u); v /= 10u; }
    while (n) { UART0_DR = (uint32_t)(unsigned char)buf[--n]; }
}

#endif /* BOARD_H */
