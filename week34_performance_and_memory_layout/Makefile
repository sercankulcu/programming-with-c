# Week 34 — Performance and Memory Layout
# Notes: https://sercankulcu.github.io/files/c/Week_34_Performance_and_Memory_Layout.html

CC      ?= cc
CFLAGS  ?= -std=c17 -Wall -Wextra -Wpedantic -g
LDLIBS  ?=

BINS := perf

all: $(BINS)

perf: perf.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

run: all
	./main

clean:
	rm -f $(BINS) *.o *.exe

.PHONY: all run clean
