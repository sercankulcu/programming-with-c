# Week 39 — Processes, Signals, and Low-Level I/O
# Notes: https://sercankulcu.github.io/files/c/Week_39_Processes_Signals_and_Low_Level_IO.html

ifeq ($(OS),Windows_NT)
all run clean:
	@echo "Week 39 uses POSIX APIs with no Windows equivalent worth teaching here."
	@echo "Run it under WSL. See the README in this directory."
.PHONY: all run clean
else

CC      ?= cc
CFLAGS  ?= -std=c17 -Wall -Wextra -Wpedantic -g
LDLIBS  ?=

BINS := main

all: $(BINS)

main: main.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

run: all
	./main

clean:
	rm -f $(BINS) *.o *.exe

.PHONY: all run clean

endif
