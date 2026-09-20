# Week 55 — Embedded Debugging and Testing
# Notes: https://sercankulcu.github.io/files/c/Week_55_Embedded_Debugging_and_Testing.html

CC      ?= arm-none-eabi-gcc
CFLAGS  ?= -mcpu=cortex-m3 -mthumb -std=c17 -Wall -Wextra -Wpedantic -g -Os -ffreestanding
LDFLAGS ?= -nostdlib

ELF := firmware.elf

all: $(ELF)

$(ELF): fuzz_protocol.c hal_device.c protocol.c test_protocol.c
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

run: $(ELF)
	qemu-system-arm -M lm3s6965evb -nographic -kernel $(ELF)

clean:
	rm -f *.elf *.o *.bin

.PHONY: all run clean
