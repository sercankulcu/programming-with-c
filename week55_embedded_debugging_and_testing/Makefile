# Week 55 - Embedded Debugging and Testing
# Notes: https://sercankulcu.github.io/files/c/Week_55_Embedded_Debugging_and_Testing.html

# The point of this week is that splitting the HAL out lets the protocol run
# on the host, where a test can drive it through a fake and a debugger costs
# nothing. So the default build is a host build with an ordinary compiler.
#
# hal_device.c is the ARM side; it is cross-compiled with the embedded weeks'
# toolchain, not here. fuzz_protocol.c needs clang: `make fuzz`.

CC      ?= cc
CFLAGS  ?= -std=c17 -Wall -Wextra -Wpedantic -g

all: test_protocol

test_protocol: test_protocol.c protocol.c
	$(CC) $(CFLAGS) $^ -o $@

run: test_protocol
	./test_protocol

fuzz: fuzz_protocol.c protocol.c
	clang -std=c17 -g -O1 -fsanitize=fuzzer,address,undefined $^ -o fuzz_protocol

device:
	@echo "hal_device.c cross-compiles with the week 49-53 toolchain:"
	@echo "  arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -c hal_device.c"

clean:
	rm -f test_protocol fuzz_protocol *.o *.exe crash-* leak-*

.PHONY: all run fuzz device clean
