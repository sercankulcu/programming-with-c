# Week 54 — Real-Time Operating Systems
# Notes: https://sercankulcu.github.io/files/c/Week_54_Real_Time_Operating_Systems.html

# superloop.c and rtos.c are the same device solved two ways. They are separate
# programs, so each links into its own image.
CC      ?= arm-none-eabi-gcc
CFLAGS  ?= -mcpu=cortex-m3 -mthumb -std=c17 -Wall -Wextra -g -Os -ffreestanding
LDFLAGS ?= -nostdlib

ELVES := superloop.elf rtos.elf main.elf

all: $(ELVES)

%.elf: %.c
	$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@

run: superloop.elf
	qemu-system-arm -M lm3s6965evb -nographic -kernel superloop.elf

clean:
	rm -f *.elf *.o

.PHONY: all run clean
