# Week 24 — The Standard Library Toolbox
# Notes: https://sercankulcu.github.io/files/c/Week_24_Standard_Library_Toolbox.html

CC      ?= cc
CFLAGS  ?= -std=c17 -Wall -Wextra -Wpedantic -g
LDLIBS  ?= -lm

BINS := main

all: $(BINS)

main: main.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

run: all
	./main

clean:
	rm -f $(BINS) *.o *.exe

.PHONY: all run clean
