# Week 37 — Portability and the Machine Model
# Notes: https://sercankulcu.github.io/files/c/Week_37_Portability_and_Machine_Model.html

CC      ?= cc
# This week uses POSIX interfaces. -std=c17 hides them behind
# __STRICT_ANSI__, and _POSIX_C_SOURCE opens them on glibc but *restricts*
# them on macOS. -std=gnu17 is the one setting that works on all three
# libcs; the language accepted is still C17.
CFLAGS  ?= -std=gnu17 -Wall -Wextra -Wpedantic -g
LDLIBS  ?=

BINS := portable

all: $(BINS)

portable: portable.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

run: all
	./portable

clean:
	rm -f $(BINS) *.o *.exe

.PHONY: all run clean
