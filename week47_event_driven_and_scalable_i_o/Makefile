# Week 47 — Event-Driven and Scalable I/O
# Notes: https://sercankulcu.github.io/files/c/Week_47_Event_Driven_and_Scalable_IO.html


UNAME_S := $(shell uname -s)

ifneq ($(UNAME_S),Linux)
all run clean:
	@echo "Week 47 uses epoll, which is Linux-only."
	@echo "On macOS or Windows, run it under a Linux container or VM."
.PHONY: all run clean
else

CC      ?= cc
# This week uses POSIX interfaces. -std=c17 hides them behind
# __STRICT_ANSI__, and _POSIX_C_SOURCE opens them on glibc but *restricts*
# them on macOS. -std=gnu17 is the one setting that works on all three
# libcs; the language accepted is still C17.
CFLAGS  ?= -std=gnu17 -Wall -Wextra -Wpedantic -g
LDLIBS  ?=

BINS := evserver

all: $(BINS)

evserver: evserver.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

run: all
	./evserver

clean:
	rm -f $(BINS) *.o *.exe

.PHONY: all run clean

endif
