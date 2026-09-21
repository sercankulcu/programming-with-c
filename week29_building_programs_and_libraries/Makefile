# Week 29 - Building Programs and Libraries
# Notes: https://sercankulcu.github.io/files/c/Week_29_Building_and_Libraries.html

# The same module from week 28, built four ways. Try `make -n` to see the
# commands without running them, and `make -j8` to watch independent
# compilations happen at once.

CC      ?= cc
CFLAGS  ?= -std=c17 -Wall -Wextra -Wpedantic -g
ASAN    ?= -fsanitize=address,undefined

OBJ := intarray.o main.o

all: app

# 1. straight build, object by object - only what changed is recompiled
app: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@

%.o: %.c intarray.h
	$(CC) $(CFLAGS) -c $< -o $@

# 2. the same thing instrumented
asan: intarray.c main.c
	$(CC) $(CFLAGS) $(ASAN) intarray.c main.c -o app-asan

# 3. a static library: the code is copied into the executable
static: intarray.c main.c
	$(CC) $(CFLAGS) -c intarray.c -o intarray.o
	ar rcs libintarray.a intarray.o
	$(CC) $(CFLAGS) main.c -L. -lintarray -o app-static

# 4. a shared library: the code stays in the .so and is found at run time
shared: intarray.c main.c
	$(CC) $(CFLAGS) -fPIC -c intarray.c -o intarray-pic.o
	$(CC) -shared intarray-pic.o -o libintarray.so
	$(CC) $(CFLAGS) main.c -L. -lintarray -o app-shared
	@echo "run it with: LD_LIBRARY_PATH=. ./app-shared"

# the linking-order failure from the notes: this one is meant to fail
wrong-order: intarray.c main.c
	$(CC) $(CFLAGS) -c intarray.c -o intarray.o
	ar rcs libintarray.a intarray.o
	-$(CC) $(CFLAGS) -L. -lintarray main.c -o app-wrong

run: app
	./app

clean:
	rm -f app app-asan app-static app-shared app-wrong *.o *.a *.so *.exe
	rm -rf build build-asan

.PHONY: all asan static shared wrong-order run clean
