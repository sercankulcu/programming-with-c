/* FreeRTOSConfig.h - FreeRTOS for rtos.c on QEMU's lm3s6965evb.
 *
 * The target is a Cortex-M3 (LM3S6965: 256 KB flash, 64 KB SRAM) with the
 * GCC ARM_CM3 port. Every setting below is either something rtos.c needs or
 * a check that turns a silent failure into a message on the UART.
 */
#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/* ---------- clock and tick ---------- */

/* QEMU's lm3s6965evb runs at 12.5 MHz out of reset (200 MHz / 16), and the
   port programs SysTick from this value. board.h says the same thing as
   BOARD_CPU_HZ; the two must agree or a tick stops being a millisecond. */
#define configCPU_CLOCK_HZ                      12500000UL
#define configTICK_RATE_HZ                      1000      /* 1 tick = 1 ms */
#define configTICK_TYPE_WIDTH_IN_BITS           TICK_TYPE_WIDTH_32_BITS

/* ---------- scheduler ---------- */

#define configUSE_PREEMPTION                    1
#define configUSE_TIME_SLICING                  1
#define configMAX_PRIORITIES                    5         /* rtos.c uses 1..4 */
#define configMINIMAL_STACK_SIZE                128       /* words: the idle task */
#define configMAX_TASK_NAME_LEN                 16
#define configIDLE_SHOULD_YIELD                 1
#define configUSE_IDLE_HOOK                     0
#define configUSE_TICK_HOOK                     0

/* ---------- what rtos.c calls ---------- */

#define configUSE_MUTEXES                       1         /* xSemaphoreCreateMutex */
#define configUSE_COUNTING_SEMAPHORES           0
#define configUSE_RECURSIVE_MUTEXES             0
#define configUSE_QUEUE_SETS                    0
#define configUSE_TASK_NOTIFICATIONS            1
#define configQUEUE_REGISTRY_SIZE               0
#define configUSE_TIMERS                        0         /* timers.c builds to nothing */
#define configUSE_CO_ROUTINES                   0

#define INCLUDE_vTaskDelay                      1
#define INCLUDE_xTaskDelayUntil                 1         /* vTaskDelayUntil */
#define INCLUDE_vTaskSuspend                    1         /* also lets portMAX_DELAY block forever */
#define INCLUDE_uxTaskGetStackHighWaterMark     1
#define INCLUDE_vTaskDelete                     0
#define INCLUDE_vTaskPrioritySet                0
#define INCLUDE_uxTaskPriorityGet               0

/* ---------- memory ---------- */

/* heap_4.c takes this as one static array in .bss. Four tasks of 256 words
   plus the idle task, two queues and a mutex need about 6 KB; 16 KB leaves
   room for the experiments in the notes, and with the 4 KB main stack it
   is well inside the 64 KB of SRAM. firmware.ld fails the link if not. */
#define configSUPPORT_DYNAMIC_ALLOCATION        1
#define configSUPPORT_STATIC_ALLOCATION         0
#define configTOTAL_HEAP_SIZE                   (16 * 1024)

/* ---------- interrupt priorities ---------- */

/* The LM3S6965 implements 3 priority bits, so only the top 3 bits of a
   priority byte mean anything. PendSV and SysTick run at the lowest
   priority; interrupts at 0xA0 (5) or numerically above may call the
   FromISR functions, anything more urgent must not. */
#define configKERNEL_INTERRUPT_PRIORITY         255
#define configMAX_SYSCALL_INTERRUPT_PRIORITY    (5 << 5)

/* The port's handlers, under the names startup.c puts in the vector table.
   With configCHECK_HANDLER_INSTALLATION the port asserts at start-up that
   the table really points at them. */
#define vPortSVCHandler                         svc_handler
#define xPortPendSVHandler                      pendsv_handler
#define xPortSysTickHandler                     systick_handler
#define configCHECK_HANDLER_INSTALLATION        1

/* ---------- catching mistakes ---------- */

/* Each of these ends in rtos_hooks.c, which prints what happened on the
   UART and stops - there is no debugger attached to tell you otherwise. */
#define configCHECK_FOR_STACK_OVERFLOW          2         /* vApplicationStackOverflowHook */
#define configUSE_MALLOC_FAILED_HOOK            1         /* vApplicationMallocFailedHook  */

void rtos_assert_failed(const char *file, int line);
#define configASSERT(x) \
    do { if ((x) == 0) { rtos_assert_failed(__FILE__, __LINE__); } } while (0)

#endif /* FREERTOS_CONFIG_H */
