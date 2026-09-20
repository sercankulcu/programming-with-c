# Week 36 — Undefined Behavior and Secure Coding
# Notes: https://sercankulcu.github.io/files/c/Week_36_Undefined_Behavior_and_Secure_Coding.html

CC      ?= cc
CFLAGS  ?= -std=c17 -Wall -Wextra -Wpedantic -g
LDLIBS  ?=

BINS := ub

all: $(BINS)

ub: ub.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

run: all
	./main

clean:
	rm -f $(BINS) *.o *.exe

.PHONY: all run clean
