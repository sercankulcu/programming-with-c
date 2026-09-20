# Week 19 — Dynamic Memory Allocation
# Notes: https://sercankulcu.github.io/files/c/Week_19_Dynamic_Memory_Allocation.html

CC      ?= cc
CFLAGS  ?= -std=c17 -Wall -Wextra -Wpedantic -g
LDLIBS  ?=

BINS := dynarray

all: $(BINS)

dynarray: dynarray.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

run: all
	./main

clean:
	rm -f $(BINS) *.o *.exe

.PHONY: all run clean
