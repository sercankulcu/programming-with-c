# Week 22 — Unions, Enumerations, and Type Aliases
# Notes: https://sercankulcu.github.io/files/c/Week_22_Unions_Enums_and_Typedef.html

CC      ?= cc
CFLAGS  ?= -std=c17 -Wall -Wextra -Wpedantic -g
LDLIBS  ?=

BINS := variants

all: $(BINS)

variants: variants.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

run: all
	./main

clean:
	rm -f $(BINS) *.o *.exe

.PHONY: all run clean
