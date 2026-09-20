# Week 28 — The Preprocessor and Modular Programming
# Notes: https://sercankulcu.github.io/files/c/Week_28_Preprocessor_and_Modular_Programming.html

CC      ?= cc
CFLAGS  ?= -std=c17 -Wall -Wextra -Wpedantic -g
LDLIBS  ?=

BINS := main

all: $(BINS)

main: main.c buffer.c intarray.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

run: all
	./main

clean:
	rm -f $(BINS) *.o *.exe

.PHONY: all run clean
