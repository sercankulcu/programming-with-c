/* board.h - the little the two schedulers need from the board.
 *
 * Same lm3s6965 UART as weeks 49-51, kept header-only so each week's image
 * stays one translation unit plus startup.
 */
#ifndef BOARD_H
#define BOARD_H

#include <stdint.h>

#define UART0_DR (*(volatile uint32_t *)0x4000C000u)

extern volatile uint32_t tick_count;

static inline void board_init(void) { tick_count = 0; }

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
