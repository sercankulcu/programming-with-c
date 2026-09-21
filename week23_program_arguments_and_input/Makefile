# Week 23 — Program Arguments and Input
# Notes: https://sercankulcu.github.io/files/c/Week_23_Program_Arguments_and_Input.html

CC      ?= cc
CFLAGS  ?= -std=c17 -Wall -Wextra -Wpedantic -g
LDLIBS  ?=

BINS := scale variant2

all: $(BINS)

scale: scale.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

variant2: variant2.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

run: all
	./scale

clean:
	rm -f $(BINS) *.o *.exe

.PHONY: all run clean
