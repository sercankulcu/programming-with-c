/* startup.c */
#include <stddef.h>
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
   struct initialisation and array copies even when you never write them.
   GCC can also recognise the loops below as memset and memcpy and replace
   them with a call - to themselves. Clang never does that inside functions
   with these names; GCC has to be told, and only GCC knows the attribute. */
#if defined(__GNUC__) && !defined(__clang__)
#define NO_LIBCALL __attribute__((optimize("no-tree-loop-distribute-patterns")))
#else
#define NO_LIBCALL
#endif

NO_LIBCALL void *memset(void *dst, int c, size_t n)
{
    unsigned char *p = dst;
    while (n--) { *p++ = (unsigned char)c; }
    return dst;
}

NO_LIBCALL void *memcpy(void *dst, const void *src, size_t n)
{
    unsigned char *d = dst;
    const unsigned char *s = src;
    while (n--) { *d++ = *s++; }
    return dst;
}
