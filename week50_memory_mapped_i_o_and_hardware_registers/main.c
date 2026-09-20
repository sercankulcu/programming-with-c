/* main.c */
#include <stdint.h>
#include <stdbool.h>

void uart_init(void);
void uart_putc(char c);
bool uart_getc(char *out);
void uart_puts(const char *s);
void uart_flush(void);

static void put_hex(uint32_t v)
{
    static const char d[] = "0123456789ABCDEF";
    uart_puts("0x");
    for (int s = 28; s >= 0; s -= 4) uart_putc(d[(v >> s) & 0xFu]);
}

/* Deliberately WITHOUT volatile, to be inspected in the disassembly. */
#define UART_FR_UNSAFE (*(uint32_t *)0x4000C018u)

static void demonstrate_without_volatile(void)
{
    uart_puts("reading FR without volatile three times:\n");
    uint32_t a = UART_FR_UNSAFE;
    uint32_t b = UART_FR_UNSAFE;
    uint32_t c = UART_FR_UNSAFE;
    uart_puts("  "); put_hex(a);
    uart_puts(" ");  put_hex(b);
    uart_puts(" ");  put_hex(c);
    uart_puts("\n  disassemble this function: at -O2 there is likely\n");
    uart_puts("  ONE load, not three\n");
}

int main(void)
{
    uart_init();

    uart_puts("\n=== memory-mapped I/O ===\n");

    uart_puts("UART base    : "); put_hex(0x4000C000u); uart_puts("\n");
    uart_puts("FR offset    : 0x018\n");
    uart_puts("FR value     : "); put_hex(*(volatile uint32_t *)0x4000C018u);
    uart_puts("\n  bit 4 RXFE set means the receive FIFO is empty\n");
    uart_puts("  bit 7 TXFE set means the transmit FIFO is empty\n\n");

    demonstrate_without_volatile();

    uart_puts("\ntype characters; they are echoed. 'q' quits to the loop.\n");

    for (;;) {
        char c;
        if (uart_getc(&c)) {
            if (c == 'q') {
                uart_puts("\nbye\n");
                break;
            }
            uart_puts("got '");
            uart_putc(c);
            uart_puts("' = ");
            put_hex((uint32_t)(unsigned char)c);
            uart_puts("\n");
        }
    }

    uart_flush();
    for (;;) { }
}
