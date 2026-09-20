# Week 33 — Trees, Hash Tables, and Abstract Data Types
# Notes: https://sercankulcu.github.io/files/c/Week_33_Trees_Hash_Tables_and_ADTs.html

CC      ?= cc
CFLAGS  ?= -std=c17 -Wall -Wextra -Wpedantic -g
LDLIBS  ?=

BINS := adt

all: $(BINS)

adt: adt.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

run: all
	./main

clean:
	rm -f $(BINS) *.o *.exe

.PHONY: all run clean
