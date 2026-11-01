/* tick.c - the superloop's 1 ms tick. Linked into superloop.elf only:
   in rtos.elf the FreeRTOS port provides systick_handler instead. */
#include "board.h"

void systick_handler(void)
{
    tick_count++;            /* 32-bit and volatile: one store, no lock */
}
