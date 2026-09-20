# Week 35 — Debugging, Testing, and Analysis
# Notes: https://sercankulcu.github.io/files/c/Week_35_Debugging_and_Testing.html

CC      ?= cc
CFLAGS  ?= -std=c17 -Wall -Wextra -Wpedantic -g
LDLIBS  ?=

BINS := tests

all: $(BINS)

tests: tests.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

run: all
	./main

clean:
	rm -f $(BINS) *.o *.exe

.PHONY: all run clean
