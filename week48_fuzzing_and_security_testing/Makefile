# Week 48 — Fuzzing and Security Testing
# Notes: https://sercankulcu.github.io/files/c/Week_48_Fuzzing_and_Security_Testing.html

# libFuzzer needs clang. The sanitizers are not optional here: without them the
# fuzzer generates crashing input and never notices.
# libFuzzer is clang-only, so this one ignores any CC from the
# environment rather than failing confusingly under gcc.
UNAME_S := $(shell uname -s)

ifneq ($(UNAME_S),Linux)
all run clean:
	@echo "Week 48 uses libFuzzer as shipped with the distribution clang, which is Linux-only."
	@echo "On macOS or Windows, run it under a Linux container or VM."
.PHONY: all run clean
else

CC     := clang
CFLAGS ?= -std=gnu17 -Wall -Wextra -g -O1 -fsanitize=fuzzer,address,undefined

all: fuzz_parser

fuzz_parser: fuzz_parser.c parser.c
	$(CC) $(CFLAGS) $^ -o $@

run: fuzz_parser
	./fuzz_parser -max_total_time=30 corpus

corpus:
	mkdir -p corpus && printf "timeout=5\nname=ok\n" > corpus/seed

clean:
	rm -f fuzz_parser *.o crash-* leak-* timeout-*
	rm -rf corpus

.PHONY: all run clean

endif
