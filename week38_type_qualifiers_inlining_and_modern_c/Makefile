# Week 38 — Type Qualifiers, Inlining, and Modern C
# Notes: https://sercankulcu.github.io/files/c/Week_38_Type_Qualifiers_and_Modern_C.html

CC      ?= cc
CFLAGS  ?= -std=c17 -Wall -Wextra -Wpedantic -g
LDLIBS  ?=

BINS := modern

all: $(BINS)

modern: modern.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

run: all
	./main

clean:
	rm -f $(BINS) *.o *.exe

.PHONY: all run clean
