/* constrained.c */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* ---------- UART (weeks 50, 52) ---------- */
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
static void put_int(int32_t v)
{
    char t[12]; int i = 0;
    if (v < 0) { putc_raw('-'); v = -v; }
    if (!v) { putc_raw('0'); return; }
    while (v) { t[i++] = (char)('0' + v % 10); v /= 10; }
    while (i) putc_raw(t[--i]);
}

/* ---------- fixed point, Q16.16 ---------- */

typedef int32_t q16_t;
#define Q16_SHIFT 16
#define Q16_ONE   (1 << Q16_SHIFT)
#define INT_TO_Q16(n) ((q16_t)((n) * Q16_ONE))

static inline q16_t q16_mul(q16_t a, q16_t b)
{
    return (q16_t)(((int64_t)a * b) >> Q16_SHIFT);
}
static inline q16_t q16_div(q16_t a, q16_t b)
{
    return (q16_t)((((int64_t)a) << Q16_SHIFT) / b);
}

/* Print a Q16.16 with three decimal places, no floating point. */
static void put_q16(q16_t q)
{
    if (q < 0) { putc_raw('-'); q = -q; }
    put_int(q >> Q16_SHIFT);
    putc_raw('.');
    uint32_t frac = (uint32_t)(q & 0xFFFFu);
    frac = (frac * 1000u) >> Q16_SHIFT;          /* to thousandths */
    if (frac < 100) putc_raw('0');
    if (frac < 10)  putc_raw('0');
    put_int((int32_t)frac);
}

/* ---------- a fixed-block pool ---------- */

#define POOL_BLOCKS     8
#define POOL_BLOCK_SIZE 32

typedef struct Block { struct Block *next; } Block;

static uint8_t pool_storage[POOL_BLOCKS * POOL_BLOCK_SIZE];
static Block  *free_list;
static uint32_t pool_in_use;
static uint32_t pool_peak;

static void pool_init(void)
{
    free_list = NULL;
    for (size_t i = 0; i < POOL_BLOCKS; i++) {
        Block *b = (Block *)(void *)&pool_storage[i * POOL_BLOCK_SIZE];
        b->next = free_list;
        free_list = b;
    }
    pool_in_use = pool_peak = 0;
}

static void *pool_alloc(void)
{
    if (free_list == NULL) return NULL;        /* exhausted, not undefined */
    Block *b = free_list;
    free_list = b->next;
    pool_in_use++;
    if (pool_in_use > pool_peak) pool_peak = pool_in_use;
    return b;
}

static void pool_free(void *p)
{
    if (p == NULL) return;
    Block *b = p;
    b->next = free_list;
    free_list = b;
    pool_in_use--;
}

/* ---------- a ring buffer: no allocation at all ---------- */

#define RING_SIZE 16
typedef struct {
    uint8_t data[RING_SIZE];
    uint8_t head, tail;
} Ring;

static bool ring_push(Ring *r, uint8_t v)
{
    uint8_t next = (uint8_t)((r->head + 1u) % RING_SIZE);
    if (next == r->tail) return false;          /* full: refuse, not grow */
    r->data[r->head] = v;
    r->head = next;
    return true;
}

static bool ring_pop(Ring *r, uint8_t *out)
{
    if (r->tail == r->head) return false;
    *out = r->data[r->tail];
    r->tail = (uint8_t)((r->tail + 1u) % RING_SIZE);
    return true;
}

/* ---------- a lookup table, in flash ---------- */

/* sin(x) scaled to Q16.16, 16 entries over a quarter turn.
   const keeps it in .rodata — week 52. */
static const q16_t sine_table[17] = {
        0,  6423, 12785, 19024, 25079, 30893, 36409, 41575,
    46340, 50660, 54491, 57797, 60547, 62714, 64276, 65220, 65536
};

static q16_t q16_sin_quarter(uint8_t index)     /* 0..16 */
{
    return sine_table[index > 16 ? 16 : index];
}

/* ---------- a PID step, entirely in fixed point ---------- */

typedef struct {
    q16_t kp, ki, kd;
    q16_t integral, previous;
} Pid;

static q16_t pid_step(Pid *p, q16_t setpoint, q16_t measured)
{
    q16_t error = setpoint - measured;
    p->integral += error;

    q16_t derivative = error - p->previous;
    p->previous = error;

    return q16_mul(p->kp, error)
         + q16_mul(p->ki, p->integral)
         + q16_mul(p->kd, derivative);
}

int main(void)
{
    puts_raw("\n=== constrained resources ===\n");

    /* --- pool --- */
    puts_raw("\nfixed-block pool: ");
    put_int(POOL_BLOCKS); puts_raw(" blocks of ");
    put_int(POOL_BLOCK_SIZE); puts_raw(" bytes\n");

    pool_init();
    void *held[POOL_BLOCKS + 2];
    size_t got = 0;

    for (size_t i = 0; i < POOL_BLOCKS + 2; i++) {
        held[i] = pool_alloc();
        puts_raw("  alloc "); put_int((int32_t)i);
        puts_raw(held[i] ? " -> ok\n" : " -> refused (pool exhausted)\n");
        if (held[i]) got++;
    }

    puts_raw("  succeeded: "); put_int((int32_t)got);
    puts_raw(", peak in use: "); put_int((int32_t)pool_peak); puts_raw("\n");

    for (size_t i = 0; i < POOL_BLOCKS + 2; i++) pool_free(held[i]);
    puts_raw("  after freeing all, in use: ");
    put_int((int32_t)pool_in_use); puts_raw("\n");

    puts_raw("  allocation is O(1) every time — no search, no coalescing,\n");
    puts_raw("  and fragmentation is impossible: every block is the same size\n");

    /* --- ring buffer --- */
    puts_raw("\nring buffer of ");
    put_int(RING_SIZE); puts_raw(" bytes, allocating nothing\n");

    Ring r = { .head = 0, .tail = 0 };
    int pushed = 0;
    for (uint8_t v = 1; v <= 20; v++) {
        if (ring_push(&r, v)) pushed++;
    }
    puts_raw("  pushed "); put_int(pushed);
    puts_raw(" of 20; the rest were refused\n  drained: ");
    uint8_t v;
    while (ring_pop(&r, &v)) { put_int(v); putc_raw(' '); }
    puts_raw("\n");

    /* --- fixed point --- */
    puts_raw("\nfixed point, Q16.16\n");

    q16_t a = INT_TO_Q16(3);
    q16_t b = q16_div(INT_TO_Q16(1), INT_TO_Q16(4));    /* 0.25 */

    puts_raw("  a        = "); put_q16(a); puts_raw("\n");
    puts_raw("  b        = "); put_q16(b); puts_raw("\n");
    puts_raw("  a * b    = "); put_q16(q16_mul(a, b)); puts_raw("\n");
    puts_raw("  a / b    = "); put_q16(q16_div(a, b)); puts_raw("\n");
    puts_raw("  a + b    = "); put_q16(a + b); puts_raw("\n");
    puts_raw("  resolution is 1/65536 everywhere, unlike a float\n");

    /* --- lookup table --- */
    puts_raw("\nsine from a flash table (no library call)\n");
    for (uint8_t i = 0; i <= 16; i += 4) {
        puts_raw("  sin("); put_int(i * 90 / 16); puts_raw(" deg) = ");
        put_q16(q16_sin_quarter(i));
        puts_raw("\n");
    }
    puts_raw("  17 entries = 68 bytes of flash, zero RAM\n");

    /* --- PID --- */
    puts_raw("\na PID controller in fixed point\n");
    Pid pid = {
        .kp = INT_TO_Q16(1),
        .ki = q16_div(INT_TO_Q16(1), INT_TO_Q16(10)),
        .kd = q16_div(INT_TO_Q16(1), INT_TO_Q16(20)),
        .integral = 0, .previous = 0
    };
    q16_t setpoint = INT_TO_Q16(100);
    q16_t measured = 0;

    for (int step = 0; step < 6; step++) {
        q16_t out = pid_step(&pid, setpoint, measured);
        measured += q16_div(out, INT_TO_Q16(10));      /* crude plant model */
        puts_raw("  step "); put_int(step);
        puts_raw(": output "); put_q16(out);
        puts_raw(", measured "); put_q16(measured);
        puts_raw("\n");
    }
    puts_raw("  no FPU, no soft-float library, constant timing\n");

    for (;;) {
        __asm__ volatile ("wfi");        /* sleep until an interrupt */
    }
}
