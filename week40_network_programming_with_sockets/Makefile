# Week 40 — Network Programming with Sockets
# Notes: https://sercankulcu.github.io/files/c/Week_40_Network_Programming_with_Sockets.html

ifeq ($(OS),Windows_NT)
all run clean:
	@echo "Week 40 uses POSIX APIs with no Windows equivalent worth teaching here."
	@echo "Run it under WSL. See the README in this directory."
.PHONY: all run clean
else

CC      ?= cc
CFLAGS  ?= -std=c17 -Wall -Wextra -Wpedantic -g
LDLIBS  ?=

BINS := echoclient echoserver

all: $(BINS)

echoclient: echoclient.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

echoserver: echoserver.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

run: all
	./echoclient

clean:
	rm -f $(BINS) *.o *.exe

.PHONY: all run clean

endif
