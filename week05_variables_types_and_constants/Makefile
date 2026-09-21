# Week 05 — Variables, Types, and Constants
# Notes: https://sercankulcu.github.io/files/c/Week_05_Variables_Types_and_Constants.html

CC      ?= cc
CFLAGS  ?= -std=c17 -Wall -Wextra -Wpedantic -g
LDLIBS  ?=

BINS := types

all: $(BINS)

types: types.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

run: all
	./types

clean:
	rm -f $(BINS) *.o *.exe

.PHONY: all run clean
