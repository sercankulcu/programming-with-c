# Week 06 — Operators and Expressions
# Notes: https://sercankulcu.github.io/files/c/Week_06_Operators_and_Expressions.html

CC      ?= cc
CFLAGS  ?= -std=c17 -Wall -Wextra -Wpedantic -g
LDLIBS  ?=

BINS := expr

all: $(BINS)

expr: expr.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

run: all
	./expr

clean:
	rm -f $(BINS) *.o *.exe

.PHONY: all run clean
