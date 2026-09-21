# Week 50 - Memory-Mapped I/O and Hardware Registers
# Notes: https://sercankulcu.github.io/files/c/Week_50_Memory_Mapped_IO_and_Registers.html

# Sources are picked up from the directory rather than listed, so the shared
# startup code and linker script carried forward from week 49 are always in
# the build.
#
# -nostdlib drops libgcc too, but the compiler still emits calls into it for
# 64-bit division, so it is linked back explicitly.

CC      ?= arm-none-eabi-gcc
CFLAGS  ?= -mcpu=cortex-m3 -mthumb -std=c17 -Wall -Wextra -Wpedantic -g -Os -ffreestanding
LDFLAGS ?= -nostdlib -T firmware.ld
LDLIBS  ?= -lgcc

SRC := $(wildcard *.c)
ELF := firmware.elf

all: $(ELF)

$(ELF): $(SRC) firmware.ld
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC) -o $@ $(LDLIBS)

run: $(ELF)
	qemu-system-arm -M lm3s6965evb -nographic -kernel $(ELF)

debug: $(ELF)
	qemu-system-arm -M lm3s6965evb -nographic -s -S -kernel $(ELF)

size: $(ELF)
	arm-none-eabi-size $(ELF)

clean:
	rm -f *.elf *.o *.bin *.map

.PHONY: all run debug size clean
