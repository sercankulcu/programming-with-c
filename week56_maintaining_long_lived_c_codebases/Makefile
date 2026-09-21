# Week 56 - Maintaining Long-Lived C Codebases
# Notes: https://sercankulcu.github.io/files/c/Week_56_Maintaining_Long_Lived_C_Codebases.html

# legacy.c is deliberately 1990s C: K&R parameter lists and implicit int.
# C23 removed both, so it is built as gnu89 - which is exactly the migration
# this week is about. Everything else is current C.

CC       ?= cc
CFLAGS   ?= -std=c17 -Wall -Wextra -Wpedantic -g
OLDFLAGS ?= -std=gnu89 -w -g

BINS := characterize test_legacy

all: $(BINS)

legacy.o: legacy.c
	$(CC) $(OLDFLAGS) -c legacy.c -o legacy.o

characterize: characterize.c legacy.o
	$(CC) $(CFLAGS) $^ -o $@

test_legacy: test_legacy.c legacy.o
	$(CC) $(CFLAGS) $^ -o $@

# the modernized replacement, built as current C
parser.o: parser.c parser.h
	$(CC) $(CFLAGS) -c parser.c -o parser.o

run: all
	./characterize

clean:
	rm -f $(BINS) *.o *.exe

.PHONY: all run clean
