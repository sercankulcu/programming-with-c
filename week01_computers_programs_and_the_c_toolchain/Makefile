# Week 01 — Computers, Programs, and the C Toolchain
# Notes: https://sercankulcu.github.io/files/c/Week_01_Computers_and_Toolchain.html

CC      ?= cc
CFLAGS  ?= -std=c17 -Wall -Wextra -Wpedantic -g
LDLIBS  ?=

BINS := hello

all: $(BINS)

hello: hello.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

run: all
	./hello

clean:
	rm -f $(BINS) *.o *.exe

.PHONY: all run clean
