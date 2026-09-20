# Week 31 — Command-Line Interfaces and the Environment
# Notes: https://sercankulcu.github.io/files/c/Week_31_Command_Line_Interfaces_and_Environment.html

ifeq ($(OS),Windows_NT)
all run clean:
	@echo "Week 31 uses POSIX APIs with no Windows equivalent worth teaching here."
	@echo "Run it under WSL. See the README in this directory."
.PHONY: all run clean
else

CC      ?= cc
CFLAGS  ?= -std=c17 -Wall -Wextra -Wpedantic -g
LDLIBS  ?=

BINS := filter

all: $(BINS)

filter: filter.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

run: all
	./main

clean:
	rm -f $(BINS) *.o *.exe

.PHONY: all run clean

endif
