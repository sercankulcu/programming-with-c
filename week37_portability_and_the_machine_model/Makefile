# Week 37 — Portability and the Machine Model
# Notes: https://sercankulcu.github.io/files/c/Week_37_Portability_and_Machine_Model.html

CC      ?= cc
CFLAGS  ?= -std=c17 -Wall -Wextra -Wpedantic -g
LDLIBS  ?=

BINS := portable

all: $(BINS)

portable: portable.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

run: all
	./main

clean:
	rm -f $(BINS) *.o *.exe

.PHONY: all run clean
