# Week 13 — Introduction to Pointers
# Notes: https://sercankulcu.github.io/files/c/Week_13_Introduction_to_Pointers.html

CC      ?= cc
CFLAGS  ?= -std=c17 -Wall -Wextra -Wpedantic -g
LDLIBS  ?=

BINS := pointers

all: $(BINS)

pointers: pointers.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

run: all
	./main

clean:
	rm -f $(BINS) *.o *.exe

.PHONY: all run clean
