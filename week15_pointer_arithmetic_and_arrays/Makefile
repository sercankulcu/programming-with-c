# Week 15 — Pointer Arithmetic and Arrays
# Notes: https://sercankulcu.github.io/files/c/Week_15_Pointer_Arithmetic_and_Arrays.html

CC      ?= cc
CFLAGS  ?= -std=c17 -Wall -Wextra -Wpedantic -g
LDLIBS  ?=

BINS := ptrmath

all: $(BINS)

ptrmath: ptrmath.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

run: all
	./ptrmath

clean:
	rm -f $(BINS) *.o *.exe

.PHONY: all run clean
