# Week 41 — Concurrency and the C Memory Model
# Notes: https://sercankulcu.github.io/files/c/Week_41_Concurrency_and_Memory_Model.html

ifeq ($(OS),Windows_NT)
all run clean:
	@echo "Week 41 uses POSIX APIs with no Windows equivalent worth teaching here."
	@echo "Run it under WSL. See the README in this directory."
.PHONY: all run clean
else

CC      ?= cc
CFLAGS  ?= -std=c17 -Wall -Wextra -Wpedantic -g
LDLIBS  ?= -lpthread

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
