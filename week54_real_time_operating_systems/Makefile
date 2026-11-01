# Week 54 - Real-Time Operating Systems
# Notes: https://sercankulcu.github.io/files/c/Week_54_Real_Time_Operating_Systems.html

# Two images of the same device, sharing startup.c, firmware.ld and the
# board layer. The source lists are written out rather than picked up with
# a wildcard, because each image needs files the other must not link:
#
#   superloop.elf  startup.c superloop.c board.c tick.c
#                  tick.c owns SysTick: the superloop counts its own ticks.
#   rtos.elf       startup.c rtos.c board.c rtos_hooks.c + FreeRTOS-Kernel
#                  the FreeRTOS port owns SVCall, PendSV and SysTick.
#
# FreeRTOS is not vendored into this repository, so rtos.elf is not built
# by default - fetch FreeRTOS-Kernel, point FREERTOS at it, and use
# `make rtos.elf FREERTOS=/path/to/FreeRTOS-Kernel`. FreeRTOSConfig.h is
# found through -I. ahead of the kernel's own headers.
#
# superloop_sketch.c.txt is the bare superloop from the notes. It calls
# functions that do not exist; it is there to read, not to compile.
#
# -nostdlib drops libgcc too, but the compiler still emits calls into it for
# 64-bit division, so it is linked back explicitly.
#
# make defines CC as `cc` before it reads this file, so `CC ?=` alone would
# never take effect and a plain `make` would reach for the host compiler.
# Replace only make's built-in default; a CC from the command line or the
# environment still wins.

ifeq ($(origin CC),default)
CC       = arm-none-eabi-gcc
endif
CFLAGS  ?= -mcpu=cortex-m3 -mthumb -std=c17 -Wall -Wextra -g -Os -ffreestanding
LDFLAGS ?= -nostdlib -T firmware.ld
LDLIBS  ?= -lgcc
FREERTOS ?=

QEMU := qemu-system-arm -M lm3s6965evb -nographic -serial mon:stdio

SUPERLOOP_SRC := startup.c superloop.c board.c tick.c
RTOS_SRC      := startup.c rtos.c board.c rtos_hooks.c
KERNEL_SRC     = $(addprefix $(FREERTOS)/, tasks.c queue.c list.c timers.c \
                   portable/GCC/ARM_CM3/port.c portable/MemMang/heap_4.c)
KERNEL_INC     = -I. -I$(FREERTOS)/include -I$(FREERTOS)/portable/GCC/ARM_CM3

all: superloop.elf

superloop.elf: $(SUPERLOOP_SRC) board.h firmware.ld
	$(CC) $(CFLAGS) $(LDFLAGS) $(SUPERLOOP_SRC) -o $@ $(LDLIBS)

rtos.elf: $(RTOS_SRC) board.h FreeRTOSConfig.h firmware.ld
	@test -n "$(FREERTOS)" || { echo "set FREERTOS=/path/to/FreeRTOS-Kernel"; exit 1; }
	@test -f "$(FREERTOS)/tasks.c" || { echo "$(FREERTOS) is not a FreeRTOS-Kernel checkout"; exit 1; }
	$(CC) $(CFLAGS) $(KERNEL_INC) $(LDFLAGS) $(RTOS_SRC) $(KERNEL_SRC) -o $@ $(LDLIBS)

run: superloop.elf
	$(QEMU) -kernel superloop.elf

run-rtos: rtos.elf
	$(QEMU) -kernel rtos.elf

debug: superloop.elf
	$(QEMU) -s -S -kernel superloop.elf

size: superloop.elf
	arm-none-eabi-size *.elf

clean:
	rm -f *.elf *.o *.bin *.map

.PHONY: all run run-rtos debug size clean
