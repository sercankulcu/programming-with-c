# Week 52 — Linker Scripts and Firmware Memory Layout
# Notes: https://sercankulcu.github.io/files/c/Week_52_Linker_Scripts_and_Firmware_Layout.html

CC      ?= arm-none-eabi-gcc
CFLAGS  ?= -mcpu=cortex-m3 -mthumb -std=c17 -Wall -Wextra -Wpedantic -g -Os -ffreestanding
LDFLAGS ?= -nostdlib

ELF := firmware.elf

all: $(ELF)

$(ELF): layout.c
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

run: $(ELF)
	qemu-system-arm -M lm3s6965evb -nographic -kernel $(ELF)

clean:
	rm -f *.elf *.o *.bin

.PHONY: all run clean
