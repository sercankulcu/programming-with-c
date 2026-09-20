# Week 08 — Formatted Input and Output
# Notes: https://sercankulcu.github.io/files/c/Week_08_Input_and_Output.html

CC      ?= cc
CFLAGS  ?= -std=c17 -Wall -Wextra -Wpedantic -g
LDLIBS  ?=

BINS := prompt

all: $(BINS)

prompt: prompt.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

run: all
	./main

clean:
	rm -f $(BINS) *.o *.exe

.PHONY: all run clean
