# Week 07 — Type Conversions
# Notes: https://sercankulcu.github.io/files/c/Week_07_Type_Conversions.html

CC      ?= cc
CFLAGS  ?= -std=c17 -Wall -Wextra -Wpedantic -g
LDLIBS  ?= -lm

BINS := convert

all: $(BINS)

convert: convert.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

run: all
	./main

clean:
	rm -f $(BINS) *.o *.exe

.PHONY: all run clean
