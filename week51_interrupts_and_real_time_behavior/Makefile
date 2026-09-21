# Week 51 — Interrupts and Real-Time Behavior
# Notes: https://sercankulcu.github.io/files/c/Week_51_Interrupts_and_Real_Time_Behavior.html

CC      ?= arm-none-eabi-gcc
CFLAGS  ?= -mcpu=cortex-m3 -mthumb -std=c17 -Wall -Wextra -Wpedantic -g -Os -ffreestanding
# -nostdlib drops libgcc too, but the compiler still emits calls into
# it for 64-bit division and for struct initialisation. Link it back.
LDLIBS  ?= -lgcc
LDFLAGS ?= -nostdlib

ELF := firmware.elf

all: $(ELF)

$(ELF): interrupts.c
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@ $(LDLIBS)

run: $(ELF)
	qemu-system-arm -M lm3s6965evb -nographic -kernel $(ELF)

clean:
	rm -f *.elf *.o *.bin

.PHONY: all run clean
