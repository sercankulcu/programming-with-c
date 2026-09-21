# Week 24 — The Standard Library Toolbox
# Notes: https://sercankulcu.github.io/files/c/Week_24_Standard_Library_Toolbox.html

CC      ?= cc
CFLAGS  ?= -std=c17 -Wall -Wextra -Wpedantic -g
LDLIBS  ?= -lm

BINS := toolbox

all: $(BINS)

toolbox: toolbox.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

run: all
	./toolbox

clean:
	rm -f $(BINS) *.o *.exe

.PHONY: all run clean
