# Week 27 — Error Handling Strategies
# Notes: https://sercankulcu.github.io/files/c/Week_27_Error_Handling.html

CC      ?= cc
CFLAGS  ?= -std=c17 -Wall -Wextra -Wpedantic -g
LDLIBS  ?=

BINS := errors

all: $(BINS)

errors: errors.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

run: all
	./main

clean:
	rm -f $(BINS) *.o *.exe

.PHONY: all run clean
