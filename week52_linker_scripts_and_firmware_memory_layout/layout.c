/* layout.c — every section populated deliberately */
#include <stdint.h>

/* Linker symbols, declared as arrays so no & is needed. */
extern uint32_t _etext[], _data_start[], _data_end[], _data_load[];
extern uint32_t _bss_start[], _bss_end[];
extern uint32_t _stack_bottom[], _stack_top[];

/* .rodata — stays in flash, costs no RAM */
static const char banner[]      = "firmware layout demo";
static const uint32_t table[16] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 };

/* .data — initial values in flash, variable in RAM */
static uint32_t marker   = 0xA5A5A5A5u;
static uint16_t revision = 42;

/* .bss — no flash cost at all */
static uint32_t counter;
static uint8_t  scratch[1024];

/* ---------- UART, from week 50 ---------- */

#define UART0_DR (*(volatile uint32_t *)0x4000C000u)
#define UART0_FR (*(volatile uint32_t *)0x4000C018u)

static void putc_raw(char c)
{
    while (UART0_FR & (1u << 5)) { }
    UART0_DR = (uint32_t)(unsigned char)c;
}
static void puts_raw(const char *s)
{
    for (; *s; s++) { if (*s == '\n') putc_raw('\r'); putc_raw(*s); }
}
static void put_hex(uint32_t v)
{
    static const char d[] = "0123456789ABCDEF";
    puts_raw("0x");
    for (int s = 28; s >= 0; s -= 4) putc_raw(d[(v >> s) & 0xFu]);
}
static void put_uint(uint32_t v)
{
    char t[11]; int i = 0;
    if (!v) { putc_raw('0'); return; }
    while (v) { t[i++] = (char)('0' + v % 10); v /= 10; }
    while (i) putc_raw(t[--i]);
}

/* ---------- stack painting ---------- */

#define PAINT 0xC0DEC0DEu

void paint_stack(void)
{
    uint32_t here;
    for (uint32_t *p = _stack_bottom; p < &here; p++) {
        *p = PAINT;
    }
}

static uint32_t stack_used_bytes(void)
{
    uint32_t *p = _stack_bottom;
    while (p < _stack_top && *p == PAINT) p++;
    return (uint32_t)((uint8_t *)_stack_top - (uint8_t *)p);
}

/* Recursion to consume stack on demand. */
static uint32_t consume(uint32_t depth)
{
    volatile uint8_t frame[64];
    frame[0] = (uint8_t)depth;
    if (depth == 0) return frame[0];
    return consume(depth - 1) + frame[0];
}

int main(void)
{
    puts_raw("\n=== firmware layout ===\n\n");

    puts_raw("FLASH\n");
    puts_raw("  .text + .rodata end : "); put_hex((uint32_t)(uintptr_t)_etext);
    puts_raw("\n  banner (rodata)     : ");
    put_hex((uint32_t)(uintptr_t)banner);
    puts_raw("\n  table  (rodata)     : ");
    put_hex((uint32_t)(uintptr_t)table);
    puts_raw("\n  .data initial values: ");
    put_hex((uint32_t)(uintptr_t)_data_load);
    puts_raw("   <-- LMA, in flash\n");

    puts_raw("\nRAM\n");
    puts_raw("  .data  "); put_hex((uint32_t)(uintptr_t)_data_start);
    puts_raw(" .. ");      put_hex((uint32_t)(uintptr_t)_data_end);
    puts_raw("   <-- VMA, in RAM\n");
    puts_raw("  .bss   "); put_hex((uint32_t)(uintptr_t)_bss_start);
    puts_raw(" .. ");      put_hex((uint32_t)(uintptr_t)_bss_end);
    puts_raw("\n  stack  "); put_hex((uint32_t)(uintptr_t)_stack_bottom);
    puts_raw(" .. ");        put_hex((uint32_t)(uintptr_t)_stack_top);
    puts_raw("\n");

    puts_raw("\nsizes\n");
    puts_raw("  .data : ");
    put_uint((uint32_t)((uint8_t *)_data_end - (uint8_t *)_data_start));
    puts_raw(" bytes  (also occupies that much FLASH)\n");
    puts_raw("  .bss  : ");
    put_uint((uint32_t)((uint8_t *)_bss_end - (uint8_t *)_bss_start));
    puts_raw(" bytes  (zero FLASH)\n");
    puts_raw("  stack : ");
    put_uint((uint32_t)((uint8_t *)_stack_top - (uint8_t *)_stack_bottom));
    puts_raw(" bytes reserved\n");

    puts_raw("\nstartup did its job\n");
    puts_raw("  marker   = "); put_hex(marker);
    puts_raw(marker == 0xA5A5A5A5u ? "  .data copied\n" : "  NOT COPIED\n");
    puts_raw("  revision = "); put_uint(revision); puts_raw("\n");
    puts_raw("  counter  = "); put_uint(counter);
    puts_raw(counter == 0 ? "  .bss zeroed\n" : "  NOT ZEROED\n");

    puts_raw("\nstack high-water mark\n");
    puts_raw("  at entry        : "); put_uint(stack_used_bytes());
    puts_raw(" bytes\n");

    (void)consume(8);
    puts_raw("  after 8 frames  : "); put_uint(stack_used_bytes());
    puts_raw(" bytes\n");

    (void)consume(40);
    puts_raw("  after 40 frames : "); put_uint(stack_used_bytes());
    puts_raw(" bytes\n");

    uint32_t reserved = (uint32_t)((uint8_t *)_stack_top -
                                   (uint8_t *)_stack_bottom);
    uint32_t used = stack_used_bytes();
    puts_raw("  headroom        : "); put_uint(reserved - used);
    puts_raw(" of "); put_uint(reserved); puts_raw(" bytes\n");
    puts_raw("\n  this is a measurement, not an estimate — which is what\n");
    puts_raw("  lets you size the stack with evidence\n");

    for (;;) { }
}
