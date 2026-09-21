# Week 25 — Bit-Level Operations
# Notes: https://sercankulcu.github.io/files/c/Week_25_Bit_Level_Operations.html

CC      ?= cc
CFLAGS  ?= -std=c17 -Wall -Wextra -Wpedantic -g
LDLIBS  ?=

BINS := bits

all: $(BINS)

bits: bits.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

run: all
	./bits

clean:
	rm -f $(BINS) *.o *.exe

.PHONY: all run clean
