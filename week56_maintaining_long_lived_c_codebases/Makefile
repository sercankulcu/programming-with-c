# Week 56 — Maintaining Long-Lived C Codebases
# Notes: https://sercankulcu.github.io/files/c/Week_56_Maintaining_Long_Lived_Codebases.html

CC      ?= cc
CFLAGS  ?= -std=c17 -Wall -Wextra -Wpedantic -g
LDLIBS  ?=

BINS := characterize test_legacy

all: $(BINS)

characterize: characterize.c legacy.c parser.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

test_legacy: test_legacy.c legacy.c parser.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

run: all
	./characterize

clean:
	rm -f $(BINS) *.o *.exe

.PHONY: all run clean
