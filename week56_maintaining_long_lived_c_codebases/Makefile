# Week 56 - Maintaining Long-Lived C Codebases
# Notes: https://sercankulcu.github.io/files/c/Week_56_Maintaining_Long_Lived_Codebases.html

# legacy.c is deliberately 1990s C: K&R parameter lists and implicit int.
# C23 removed both, so it is built as gnu89 - which is exactly the migration
# this week is about. Everything else is current C.
#
# pinned.h is not written by hand. characterize runs the legacy code over a
# corpus and prints what it currently does; test_legacy then holds it to
# that. This is the whole idea of a characterization test, so the build
# generates it rather than shipping it.

CC       ?= cc
CFLAGS   ?= -std=c17 -Wall -Wextra -Wpedantic -g
OLDFLAGS ?= -std=gnu89 -w -g

all: characterize test_legacy

legacy.o: legacy.c legacy.h
	$(CC) $(OLDFLAGS) -c legacy.c -o legacy.o

characterize: characterize.c legacy.o
	$(CC) $(CFLAGS) -w $^ -o $@

pinned.h: characterize
	./characterize > pinned.h

test_legacy: test_legacy.c legacy.o pinned.h
	$(CC) $(CFLAGS) -w test_legacy.c legacy.o -o $@

# the modernized replacement, built as current C
parser.o: parser.c parser.h
	$(CC) $(CFLAGS) -c parser.c -o parser.o

run: all
	./test_legacy

clean:
	rm -f characterize test_legacy pinned.h *.o *.exe

.PHONY: all run clean
