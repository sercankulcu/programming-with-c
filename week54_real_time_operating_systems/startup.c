/* startup.c - shared by superloop.elf and rtos.elf */
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

    /* 2. zero .bss - this includes the FreeRTOS heap, a static array */
    for (uint32_t *p = &_bss_start; p < &_bss_end; ) {
        *p++ = 0;
    }

    /* 3. only now may C code with globals run */
    main();

    for (;;) { }                 /* main must never return */
}

static void default_handler(void) { for (;;) { } }

/* Weak: each image overrides the ones it needs and the rest stay parked.
   superloop.elf gets systick_handler from tick.c; rtos.elf gets all three
   from the FreeRTOS Cortex-M3 port, which FreeRTOSConfig.h renames to these
   names. Neither image may define a handler the other one also links. */
void svc_handler(void)     __attribute__((weak, alias("default_handler")));
void pendsv_handler(void)  __attribute__((weak, alias("default_handler")));
void systick_handler(void) __attribute__((weak, alias("default_handler")));

/* The vector table: the processor reads [0] and [1] at reset. Week 49 got
   away with four entries; a scheduler needs the whole core block, because
   SVCall, PendSV and SysTick sit at [11], [14] and [15]. */
__attribute__((section(".vectors"), used))
void (* const vector_table[])(void) = {
    (void (*)(void))&_stack_top,   /* [0]  initial stack pointer           */
    reset_handler,                 /* [1]  reset                           */
    default_handler,               /* [2]  NMI                             */
    default_handler,               /* [3]  hard fault                      */
    default_handler,               /* [4]  memory management fault         */
    default_handler,               /* [5]  bus fault                       */
    default_handler,               /* [6]  usage fault                     */
    0, 0, 0, 0,                    /* [7..10] reserved                     */
    svc_handler,                   /* [11] SVCall: starts the first task   */
    default_handler,               /* [12] debug monitor                   */
    0,                             /* [13] reserved                        */
    pendsv_handler,                /* [14] PendSV: the context switch      */
    systick_handler,               /* [15] SysTick: the tick               */
};

/* Freestanding builds still need these: the compiler emits calls to them for
   struct initialisation and array copies even when you never write them,
   and the FreeRTOS kernel calls them by name. GCC can also recognise the
   loops below as memset and memcpy and replace them with a call - to
   themselves. Clang never does that inside functions with these names; GCC
   has to be told, and only GCC knows the attribute. */
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
