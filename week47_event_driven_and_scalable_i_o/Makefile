# Week 47 — Event-Driven and Scalable I/O
# Notes: https://sercankulcu.github.io/files/c/Week_47_Event_Driven_and_Scalable_IO.html

ifeq ($(OS),Windows_NT)
all run clean:
	@echo "Week 47 uses POSIX APIs with no Windows equivalent worth teaching here."
	@echo "Run it under WSL. See the README in this directory."
.PHONY: all run clean
else

CC      ?= cc
CFLAGS  ?= -std=c17 -Wall -Wextra -Wpedantic -g
LDLIBS  ?=

BINS := evserver

all: $(BINS)

evserver: evserver.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

run: all
	./main

clean:
	rm -f $(BINS) *.o *.exe

.PHONY: all run clean

endif
