# Week 14 — Strings and Character Handling
# Notes: https://sercankulcu.github.io/files/c/Week_14_Strings_and_Character_Handling.html

CC      ?= cc
CFLAGS  ?= -std=c17 -Wall -Wextra -Wpedantic -g
LDLIBS  ?=

BINS := strings

all: $(BINS)

strings: strings.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

run: all
	./main

clean:
	rm -f $(BINS) *.o *.exe

.PHONY: all run clean
