/* rtos_hooks.c - what the kernel calls when something has gone wrong.
 *
 * Linked into rtos.elf only. FreeRTOSConfig.h turns these on; each one
 * says what happened on the UART and stops the machine, which beats a
 * device that carries on with a corrupted stack.
 */
#include "FreeRTOS.h"
#include "task.h"
#include "board.h"

static void stop(void)
{
    __asm__ volatile ("cpsid i" ::: "memory");   /* nothing runs after this */
    for (;;) { }
}

/* configASSERT */
void rtos_assert_failed(const char *file, int line)
{
    __asm__ volatile ("cpsid i" ::: "memory");
    uart_puts("\n*** configASSERT failed: ");
    uart_puts(file);
    uart_puts(":");
    put_int((uint32_t)line);
    uart_puts("\n");
    stop();
}

/* configCHECK_FOR_STACK_OVERFLOW 2: checked on every context switch */
void vApplicationStackOverflowHook(TaskHandle_t task, char *name)
{
    (void)task;
    __asm__ volatile ("cpsid i" ::: "memory");
    uart_puts("\n*** stack overflow in task '");
    uart_puts(name);
    uart_puts("'\n");
    stop();
}

/* configUSE_MALLOC_FAILED_HOOK: configTOTAL_HEAP_SIZE is too small */
void vApplicationMallocFailedHook(void)
{
    __asm__ volatile ("cpsid i" ::: "memory");
    uart_puts("\n*** FreeRTOS heap exhausted\n");
    stop();
}
