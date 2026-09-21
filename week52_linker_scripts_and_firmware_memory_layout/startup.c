/* startup.c */
#include <stdint.h>

extern uint32_t _stack_top;
extern uint32_t _data_load, _data_start, _data_end;
extern uint32_t _bss_start, _bss_end;

int main(void);

void reset_handler(void)
{
    /* 1. copy .data from flash to RAM */
    uint32_t *src = &_data_load;
    for (uint32_t *dst = &_data_start; dst < &_data_end; ) {
        *dst++ = *src++;
    }

    /* 2. zero .bss */
    for (uint32_t *p = &_bss_start; p < &_bss_end; ) {
        *p++ = 0;
    }

    /* 3. only now may C code with globals run */
    main();

    for (;;) { }                 /* main must never return */
}

static void default_handler(void) { for (;;) { } }

/* The vector table: the processor reads [0] and [1] at reset. */
__attribute__((section(".vectors"), used))
void (* const vector_table[])(void) = {
    (void (*)(void))&_stack_top,   /* [0] initial stack pointer */
    reset_handler,                 /* [1] reset                 */
    default_handler,               /* NMI                       */
    default_handler,               /* hard fault                */
};

/* Freestanding builds still need these: the compiler emits calls to them for
   struct initialisation and array copies even when you never write them. */
void *memset(void *dst, int c, unsigned long n)
{
    unsigned char *p = dst;
    while (n--) { *p++ = (unsigned char)c; }
    return dst;
}

void *memcpy(void *dst, const void *src, unsigned long n)
{
    unsigned char *d = dst;
    const unsigned char *s = src;
    while (n--) { *d++ = *s++; }
    return dst;
}
