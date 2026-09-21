# Week 44 - Collaborative Development and Code Review
# Notes: https://sercankulcu.github.io/files/c/Week_44_Collaborative_Development_and_Code_Review.html

# There is no program here. review.diff is the change as it was submitted;
# config.c is the version that came out of the review. Read the diff, form
# your own review, then compare it with the one in the notes.
#
# config.c still compiles on its own, because "does it survive -Wall -Wextra"
# is part of reviewing it. strtok_r is POSIX, hence gnu17.

CC      ?= cc
CFLAGS  ?= -std=gnu17 -Wall -Wextra -Wpedantic -g

all: config.o

config.o: config.c config.h
	$(CC) $(CFLAGS) -c config.c -o config.o

review:
	@echo "Read review.diff, then config.c. The written review is in the notes."

clean:
	rm -f *.o

.PHONY: all review clean
