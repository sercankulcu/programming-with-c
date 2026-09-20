# Week 11 — Functions
# Notes: https://sercankulcu.github.io/files/c/Week_11_Functions.html

CC      ?= cc
CFLAGS  ?= -std=c17 -Wall -Wextra -Wpedantic -g
LDLIBS  ?=

BINS := main variant2 variant3

all: $(BINS)

main: main.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

variant2: variant2.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

variant3: variant3.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

run: all
	./main

clean:
	rm -f $(BINS) *.o *.exe

.PHONY: all run clean
