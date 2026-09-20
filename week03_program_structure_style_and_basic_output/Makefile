# Week 03 — Program Structure, Style, and Basic Output
# Notes: https://sercankulcu.github.io/files/c/Week_03_Program_Structure_and_Style.html

CC      ?= cc
CFLAGS  ?= -std=c17 -Wall -Wextra -Wpedantic -g
LDLIBS  ?=

BINS := main variant2 variant3 variant4

all: $(BINS)

main: main.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

variant2: variant2.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

variant3: variant3.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

variant4: variant4.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

run: all
	./main

clean:
	rm -f $(BINS) *.o *.exe

.PHONY: all run clean
