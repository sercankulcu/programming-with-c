# Week 18 — Recursion
# Notes: https://sercankulcu.github.io/files/c/Week_18_Recursion.html

CC      ?= cc
CFLAGS  ?= -std=c17 -Wall -Wextra -Wpedantic -g
LDLIBS  ?=

BINS := recur

all: $(BINS)

recur: recur.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

run: all
	./recur

clean:
	rm -f $(BINS) *.o *.exe

.PHONY: all run clean
