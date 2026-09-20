# Week 26 — Files and Streams
# Notes: https://sercankulcu.github.io/files/c/Week_26_Files_and_Streams.html

CC      ?= cc
CFLAGS  ?= -std=c17 -Wall -Wextra -Wpedantic -g
LDLIBS  ?=

BINS := files

all: $(BINS)

files: files.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

run: all
	./main

clean:
	rm -f $(BINS) *.o *.exe

.PHONY: all run clean
