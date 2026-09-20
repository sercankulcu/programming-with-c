# Week 42 — Reading Excellent C
# Notes: https://sercankulcu.github.io/files/c/Week_42_Reading_Excellent_C.html

CC      ?= cc
CFLAGS  ?= -std=c17 -Wall -Wextra -Wpedantic -g
LDLIBS  ?=

BINS := bench

all: $(BINS)

bench: bench.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

run: all
	./bench

clean:
	rm -f $(BINS) *.o *.exe

.PHONY: all run clean
