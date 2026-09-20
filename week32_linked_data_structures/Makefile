# Week 32 — Linked Data Structures
# Notes: https://sercankulcu.github.io/files/c/Week_32_Linked_Data_Structures.html

CC      ?= cc
CFLAGS  ?= -std=c17 -Wall -Wextra -Wpedantic -g
LDLIBS  ?=

BINS := main

all: $(BINS)

main: main.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

run: all
	./main

clean:
	rm -f $(BINS) *.o *.exe

.PHONY: all run clean
