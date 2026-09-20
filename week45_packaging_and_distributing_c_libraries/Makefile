# Week 45 — Packaging and Distributing C Libraries
# Notes: https://sercankulcu.github.io/files/c/Week_45_Packaging_and_Distributing_Libraries.html

CC      ?= cc
CFLAGS  ?= -std=c17 -Wall -Wextra -Wpedantic -g
LDLIBS  ?=

BINS := app

all: $(BINS)

app: app.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

run: all
	./main

clean:
	rm -f $(BINS) *.o *.exe

.PHONY: all run clean
