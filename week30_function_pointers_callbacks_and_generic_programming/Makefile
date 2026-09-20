# Week 30 — Function Pointers, Callbacks, and Generic Programming
# Notes: https://sercankulcu.github.io/files/c/Week_30_Function_Pointers_and_Generic_Programming.html

CC      ?= cc
CFLAGS  ?= -std=c17 -Wall -Wextra -Wpedantic -g
LDLIBS  ?= -lm

BINS := main

all: $(BINS)

main: main.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

run: all
	./main

clean:
	rm -f $(BINS) *.o *.exe

.PHONY: all run clean
