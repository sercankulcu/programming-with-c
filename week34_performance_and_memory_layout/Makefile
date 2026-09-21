# Week 34 — Performance and Memory Layout
# Notes: https://sercankulcu.github.io/files/c/Week_34_Performance_and_Memory_Layout.html

CC      ?= cc
# This week uses POSIX interfaces. -std=c17 hides them behind
# __STRICT_ANSI__, and _POSIX_C_SOURCE opens them on glibc but *restricts*
# them on macOS. -std=gnu17 is the one setting that works on all three
# libcs; the language accepted is still C17.
CFLAGS  ?= -std=gnu17 -Wall -Wextra -Wpedantic -g
LDLIBS  ?=

BINS := perf

all: $(BINS)

perf: perf.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

run: all
	./perf

clean:
	rm -f $(BINS) *.o *.exe

.PHONY: all run clean
