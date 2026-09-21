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
# -nostdlib drops libgcc too, but the compiler still emits calls into
# it for 64-bit division and for struct initialisation. Link it back.
LDLIBS  ?= -lgcc
LDFLAGS ?= -nostdlib
FREERTOS ?=

all: superloop.elf

superloop.elf: superloop.c board.c
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@ $(LDLIBS)

rtos.elf: rtos.c board.c
	@test -n "$(FREERTOS)" || { echo "set FREERTOS=/path/to/FreeRTOS-Kernel"; exit 1; }
	$(CC) $(CFLAGS) -I$(FREERTOS)/include $(LDFLAGS) $^ -o $@ $(LDLIBS)

run: superloop.elf
	qemu-system-arm -M lm3s6965evb -nographic -kernel superloop.elf

clean:
	rm -f *.elf *.o

.PHONY: all run clean
