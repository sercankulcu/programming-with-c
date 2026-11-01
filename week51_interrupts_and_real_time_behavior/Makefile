# Week 51 - Interrupts and Real-Time Behavior
# Notes: https://sercankulcu.github.io/files/c/Week_51_Interrupts_and_Real_Time_Behavior.html

# Sources are picked up from the directory rather than listed, so the shared
# startup code and linker script carried forward from week 49 are always in
# the build.
#
# -nostdlib drops libgcc too, but the compiler still emits calls into it for
# 64-bit division, so it is linked back explicitly.

# make defines CC as `cc` before it reads this file, so `CC ?=` alone would
# never take effect and a plain `make` would reach for the host compiler.
# Replace only make's built-in default; a CC from the command line or the
# environment still wins.

ifeq ($(origin CC),default)
CC       = arm-none-eabi-gcc
endif
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
