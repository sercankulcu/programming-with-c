# Week 49 — Embedded C and Cross-Compilation
# Notes: https://sercankulcu.github.io/files/c/Week_49_Embedded_C_and_Cross_Compilation.html

CC      ?= arm-none-eabi-gcc
CFLAGS  ?= -mcpu=cortex-m3 -mthumb -std=c17 -Wall -Wextra -Wpedantic -g -Os -ffreestanding
LDFLAGS ?= -nostdlib -T firmware.ld

ELF := firmware.elf

all: $(ELF)

$(ELF): main.c startup.c
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

run: $(ELF)
	qemu-system-arm -M lm3s6965evb -nographic -kernel $(ELF)

clean:
	rm -f *.elf *.o *.bin

.PHONY: all run clean
