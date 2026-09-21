# Week 20 — Memory Errors and Their Diagnosis
# Notes: https://sercankulcu.github.io/files/c/Week_20_Memory_Errors.html

CC      ?= cc
CFLAGS  ?= -std=c17 -Wall -Wextra -Wpedantic -g
LDLIBS  ?=

BINS := bugs

all: $(BINS)

bugs: bugs.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

run: all
	./bugs

clean:
	rm -f $(BINS) *.o *.exe

.PHONY: all run clean
