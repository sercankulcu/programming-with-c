# Week 03 — Program Structure, Style, and Basic Output
# Notes: https://sercankulcu.github.io/files/c/Week_03_Program_Structure_and_Style.html

CC      ?= cc
CFLAGS  ?= -std=c17 -Wall -Wextra -Wpedantic -g
LDLIBS  ?=

# structure.c  the annotated skeleton
# hello.c      the same program with nothing added
# ugly.c       the version this week asks you to read
# clean.c      the same program, made readable
BINS := structure hello ugly clean

all: $(BINS)

structure: structure.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

hello: hello.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

ugly: ugly.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

clean_prog: clean.c
	$(CC) $(CFLAGS) $^ -o clean_prog $(LDLIBS)

run: all
	./structure

clean:
	rm -f $(BINS) clean_prog *.o *.exe

.PHONY: all run clean
