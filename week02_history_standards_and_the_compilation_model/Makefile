# Week 02 — History, Standards, and the Compilation Model
# Notes: https://sercankulcu.github.io/files/c/Week_02_History_Standards_and_Compilation.html

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
