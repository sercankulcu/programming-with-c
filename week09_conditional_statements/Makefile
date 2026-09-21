# Week 09 — Conditional Statements
# Notes: https://sercankulcu.github.io/files/c/Week_09_Conditional_Statements.html

CC      ?= cc
CFLAGS  ?= -std=c17 -Wall -Wextra -Wpedantic -g
LDLIBS  ?=

BINS := grades

all: $(BINS)

grades: grades.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

run: all
	./grades

clean:
	rm -f $(BINS) *.o *.exe

.PHONY: all run clean
