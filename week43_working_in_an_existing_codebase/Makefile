# Week 43 — Working in an Existing Codebase
# Notes: https://sercankulcu.github.io/files/c/Week_43_Working_in_an_Existing_Codebase.html

CC      ?= cc
CFLAGS  ?= -std=c17 -Wall -Wextra -Wpedantic -g
LDLIBS  ?=

BINS := stats

all: $(BINS)

stats: stats.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

run: all
	./stats

clean:
	rm -f $(BINS) *.o *.exe

.PHONY: all run clean
