/* interrupts.c */
#include <stdint.h>
#include <stdbool.h>

/* ---------- registers (week 50) ---------- */

typedef struct {
    volatile uint32_t DR, RSR_ECR;
    uint32_t r0[4];
    volatile uint32_t FR;
    uint32_t r1;
    volatile uint32_t ILPR, IBRD, FBRD, LCRH, CTL, IFLS, IM, RIS, MIS, ICR;
} UART_Type;

#define UART0 ((UART_Type *)0x4000C000u)
#define FR_RXFE (1u << 4)
#define FR_TXFF (1u << 5)
#define IM_RXIM (1u << 4)
#define ICR_RXIC (1u << 4)

/* SysTick, in the core peripheral region */
#define SYST_CSR  (*(volatile uint32_t *)0xE000E010u)
#define SYST_RVR  (*(volatile uint32_t *)0xE000E014u)
#define SYST_CVR  (*(volatile uint32_t *)0xE000E018u)

/* NVIC */
#define NVIC_ISER0 (*(volatile uint32_t *)0xE000E100u)
#define UART0_IRQ  5

/* ---------- shared state ---------- */

/* 32-bit and volatile: a single load or store on this machine. */
static volatile uint32_t tick_count = 0;

/* 64-bit: NOT atomic on a 32-bit core. Needs a critical section. */
static volatile uint64_t micros = 0;

/* Single-producer single-consumer ring: no critical section needed. */
#define RX_SIZE 64
static volatile uint8_t rx_buf[RX_SIZE];
static volatile uint8_t rx_head = 0;      /* ISR writes  */
static volatile uint8_t rx_tail = 0;      /* main writes */

static volatile uint32_t isr_entries = 0;
static volatile uint32_t rx_dropped  = 0;

/* ---------- critical sections ---------- */

static inline uint32_t enter_critical(void)
{
    uint32_t primask;
    __asm__ volatile ("mrs %0, primask" : "=r"(primask));
    __asm__ volatile ("cpsid i" ::: "memory");
    return primask;
}

static inline void exit_critical(uint32_t primask)
{
    if ((primask & 1u) == 0) {
        __asm__ volatile ("cpsie i" ::: "memory");
    }
}

/* ---------- handlers: short, and they acknowledge ---------- */

void systick_handler(void)
{
    tick_count++;
    micros += 1000;          /* read-modify-write of a 64-bit value,
                                but only this ISR touches it */
}

void uart0_handler(void)
{
    isr_entries++;

    while ((UART0->FR & FR_RXFE) == 0) {          /* drain the FIFO */
        uint8_t byte = (uint8_t)(UART0->DR & 0xFFu);

        uint8_t next = (uint8_t)((rx_head + 1u) % RX_SIZE);
        if (next == rx_tail) {
            rx_dropped++;                          /* buffer full */
        } else {
            rx_buf[rx_head] = byte;
            rx_head = next;                        /* publish after the data */
        }
    }

    UART0->ICR = ICR_RXIC;    /* acknowledge, or we re-enter forever */
}

/* ---------- consumer side ---------- */

static bool rx_get(uint8_t *out)
{
    if (rx_tail == rx_head) return false;          /* empty */
    *out = rx_buf[rx_tail];
    rx_tail = (uint8_t)((rx_tail + 1u) % RX_SIZE);
    return true;
}

/* ---------- output ---------- */

static void putc_raw(char c)
{
    while (UART0->FR & FR_TXFF) { }
    UART0->DR = (uint32_t)(unsigned char)c;
}

static void puts_raw(const char *s)
{
    for (; *s; s++) { if (*s == '\n') putc_raw('\r'); putc_raw(*s); }
}

static void put_uint(uint32_t v)
{
    char t[11]; int i = 0;
    if (v == 0) { putc_raw('0'); return; }
    while (v) { t[i++] = (char)('0' + v % 10); v /= 10; }
    while (i) putc_raw(t[--i]);
}

static void put_u64(uint64_t v)
{
    char t[21]; int i = 0;
    if (v == 0) { putc_raw('0'); return; }
    while (v) { t[i++] = (char)('0' + (uint32_t)(v % 10)); v /= 10; }
    while (i) putc_raw(t[--i]);
}

/* ---------- setup ---------- */

static void systick_init(uint32_t reload)
{
    SYST_RVR = reload - 1u;
    SYST_CVR = 0;
    SYST_CSR = 0x7u;            /* enable, interrupt, processor clock */
}

static void uart_irq_init(void)
{
    UART0->IM  = IM_RXIM;       /* interrupt when a byte arrives */
    UART0->ICR = 0x7FFu;        /* clear anything pending */
    NVIC_ISER0 = (1u << UART0_IRQ);
}

int main(void)
{
    UART0->CTL = 0;
    UART0->IBRD = 10; UART0->FBRD = 54;
    UART0->LCRH = (3u << 5) | (1u << 4);
    UART0->CTL = 1u | (1u << 8) | (1u << 9);

    puts_raw("\n=== interrupts ===\n");

    /* --- 1. polling: the processor is fully occupied --- */
    puts_raw("\npolling for a character (type one):\n");
    uint32_t spins = 0;
    while (UART0->FR & FR_RXFE) {
        spins++;                       /* every cycle wasted */
    }
    uint8_t first = (uint8_t)(UART0->DR & 0xFFu);
    puts_raw("  got '"); putc_raw((char)first);
    puts_raw("' after "); put_uint(spins); puts_raw(" wasted iterations\n");

    /* --- 2. interrupts: the loop does other work --- */
    systick_init(18000);               /* about 1 kHz */
    uart_irq_init();
    puts_raw("\ninterrupts enabled; type freely, 'q' ends\n");

    uint32_t loop_iterations = 0;
    uint32_t received = 0;
    bool     done = false;

    while (!done) {
        loop_iterations++;             /* real work would go here */

        uint8_t byte;
        while (rx_get(&byte)) {
            received++;
            if (byte == 'q') { done = true; break; }
            puts_raw("  rx '"); putc_raw((char)byte);
            puts_raw("' at tick "); put_uint(tick_count); puts_raw("\n");
        }

        if (tick_count % 5000 == 0 && tick_count > 0) {
            puts_raw("  ... 5000 ticks, loop still running\n");
            while (tick_count % 5000 == 0) { }   /* print once */
        }
    }

    /* --- 3. the 64-bit read needs protection --- */
    puts_raw("\nreading a 64-bit counter:\n");

    uint64_t unsafe = micros;                    /* two loads, interruptible */
    puts_raw("  without a critical section: "); put_u64(unsafe);
    puts_raw("\n");

    uint32_t state = enter_critical();
    uint64_t safe = micros;                      /* indivisible */
    exit_critical(state);
    puts_raw("  with one                  : "); put_u64(safe);
    puts_raw("\n  on a 32-bit core the first is two loads; an ISR\n");
    puts_raw("  between them yields a value that never existed\n");

    /* --- 4. what it cost --- */
    puts_raw("\nstatistics\n");
    puts_raw("  ticks           : "); put_uint(tick_count);  puts_raw("\n");
    puts_raw("  uart interrupts : "); put_uint(isr_entries); puts_raw("\n");
    puts_raw("  bytes received  : "); put_uint(received);    puts_raw("\n");
    puts_raw("  bytes dropped   : "); put_uint(rx_dropped);  puts_raw("\n");
    puts_raw("  loop iterations : "); put_uint(loop_iterations);
    puts_raw("\n  the loop kept running the whole time — that is the point\n");

    for (;;) { }
}
