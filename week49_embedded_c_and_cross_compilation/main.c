/* main.c */
#include <stdint.h>

/* The UART, straight from the machine's memory map. Week 50
   explains volatile properly; note it is not optional here. */
#define UART0_BASE 0x4000C000u
#define UART_DR   (*(volatile uint32_t *)(UART0_BASE + 0x000))
#define UART_FR   (*(volatile uint32_t *)(UART0_BASE + 0x018))
#define UART_FR_TXFF (1u << 5)          /* transmit FIFO full */

/* Initialized: lives in .data, copied from flash by the startup code. */
static uint32_t boot_marker = 0xDEADBEEF;

/* Zero-initialized: lives in .bss, cleared by the startup code. */
static uint32_t counter;
static uint8_t  buffer[256];

static void uart_putc(char c)
{
    while (UART_FR & UART_FR_TXFF) { }   /* wait for space */
    UART_DR = (uint32_t)c;
}

static void uart_puts(const char *s)
{
    for (; *s != '\0'; s++) {
        if (*s == '\n') uart_putc('\r');  /* terminals want CR LF */
        uart_putc(*s);
    }
}

/* No printf here: write what you need. */
static void uart_put_hex(uint32_t v)
{
    static const char digits[] = "0123456789ABCDEF";
    uart_puts("0x");
    for (int shift = 28; shift >= 0; shift -= 4) {
        uart_putc(digits[(v >> shift) & 0xFu]);
    }
}

static void uart_put_uint(uint32_t v)
{
    char tmp[11];
    int i = 0;
    if (v == 0) { uart_putc('0'); return; }
    while (v > 0) { tmp[i++] = (char)('0' + v % 10); v /= 10; }
    while (i > 0) uart_putc(tmp[--i]);
}

int main(void)
{
    uart_puts("\n=== bare metal, no operating system ===\n");

    uart_puts(".data was copied : ");
    uart_put_hex(boot_marker);
    uart_puts(boot_marker == 0xDEADBEEF ? "  correct\n" : "  WRONG\n");

    uart_puts(".bss was zeroed  : counter = ");
    uart_put_uint(counter);
    uart_puts(counter == 0 ? "  correct\n" : "  WRONG\n");

    uart_puts("buffer[0]        : ");
    uart_put_uint(buffer[0]);
    uart_puts("\n");

    uart_puts("stack address    : ");
    uint32_t local = 0;
    uart_put_hex((uint32_t)(uintptr_t)&local);
    uart_puts("  (RAM, near the top)\n");

    uart_puts("code address     : ");
    uart_put_hex((uint32_t)(uintptr_t)main);
    uart_puts("  (flash, low)\n");

    uart_puts("\nsuperloop starts; Ctrl-A X to quit QEMU\n");
    for (;;) {
        counter++;
        if (counter % 2000000u == 0) {
            uart_puts("tick ");
            uart_put_uint(counter / 2000000u);
            uart_puts("\n");
        }
    }
}
