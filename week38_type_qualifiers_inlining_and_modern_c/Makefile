# Week 38 — Type Qualifiers, Inlining, and Modern C
# Notes: https://sercankulcu.github.io/files/c/Week_38_Type_Qualifiers_and_Modern_C.html

CC      ?= cc
# This week uses POSIX interfaces. -std=c17 hides them behind
# __STRICT_ANSI__, and _POSIX_C_SOURCE opens them on glibc but *restricts*
# them on macOS. -std=gnu17 is the one setting that works on all three
# libcs; the language accepted is still C17.
CFLAGS  ?= -std=gnu17 -Wall -Wextra -Wpedantic -g
LDLIBS  ?=

BINS := modern

all: $(BINS)

modern: modern.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

run: all
	./modern

clean:
	rm -f $(BINS) *.o *.exe

.PHONY: all run clean
