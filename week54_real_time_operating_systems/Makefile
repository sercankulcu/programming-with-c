# Week 54 - Real-Time Operating Systems
# Notes: https://sercankulcu.github.io/files/c/Week_54_Real_Time_Operating_Systems.html

# superloop.c is the same device without a scheduler; it builds here.
#
# rtos.c is the FreeRTOS version. FreeRTOS is not vendored into this
# repository, so it is not built by default - fetch FreeRTOS-Kernel, point
# FREERTOS at it, and use `make rtos.elf`.
#
# superloop_sketch.c.txt is the bare superloop from the notes. It calls
# functions that do not exist; it is there to read, not to compile.

CC      ?= arm-none-eabi-gcc
CFLAGS  ?= -mcpu=cortex-m3 -mthumb -std=c17 -Wall -Wextra -g -Os -ffreestanding
LDFLAGS ?= -nostdlib
FREERTOS ?=

all: superloop.elf

superloop.elf: superloop.c board.c
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

rtos.elf: rtos.c board.c
	@test -n "$(FREERTOS)" || { echo "set FREERTOS=/path/to/FreeRTOS-Kernel"; exit 1; }
	$(CC) $(CFLAGS) -I$(FREERTOS)/include $(LDFLAGS) $^ -o $@

run: superloop.elf
	qemu-system-arm -M lm3s6965evb -nographic -kernel superloop.elf

clean:
	rm -f *.elf *.o

.PHONY: all run clean
