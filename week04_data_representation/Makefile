# Week 04 — Data Representation
# Notes: https://sercankulcu.github.io/files/c/Week_04_Data_Representation.html

CC      ?= cc
CFLAGS  ?= -std=c17 -Wall -Wextra -Wpedantic -g
LDLIBS  ?=

BINS := bits variant2

all: $(BINS)

bits: bits.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

variant2: variant2.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

run: all
	./main

clean:
	rm -f $(BINS) *.o *.exe

.PHONY: all run clean
