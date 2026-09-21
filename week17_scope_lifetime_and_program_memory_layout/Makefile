# Week 17 — Scope, Lifetime, and Program Memory Layout
# Notes: https://sercankulcu.github.io/files/c/Week_17_Scope_and_Program_Memory_Layout.html

CC      ?= cc
CFLAGS  ?= -std=c17 -Wall -Wextra -Wpedantic -g
LDLIBS  ?=

BINS := layout

all: $(BINS)

layout: layout.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

run: all
	./layout

clean:
	rm -f $(BINS) *.o *.exe

.PHONY: all run clean
