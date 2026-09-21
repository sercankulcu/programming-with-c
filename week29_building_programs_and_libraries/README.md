# Week 29 — Building Programs and Libraries

📖 **[Lecture notes](https://sercankulcu.github.io/files/c/Week_29_Building_and_Libraries.html)** — read these first; the code here is the worked example from that page.

## Files

- `intarray.h`, `intarray.c` — week 28's module, unchanged
- `main.c` — a caller that only sees the header
- `Makefile` — the four builds below
- `CMakeLists.txt` — the same project, described declaratively

## Build and run

```bash
make            # 1. straight build, object by object
make asan       # 2. the same thing instrumented
make static     # 3. into a static library
make shared     # 4. into a shared library
make wrong-order  # the linking-order failure, on purpose
make -n         # print the commands without running them
```

## Experiments from the notes

These are the commands the lecture notes ask you to run — including the ones that
are supposed to fail.

```bash
make            # build
make -j8        # build with eight parallel jobs
make asan       # build instrumented
make clean      # remove products
make -n         # show the commands without running them
```

```bash
cmake -S . -B build
cmake --build build -j8
cmake -S . -B build-asan -DENABLE_ASAN=ON
cmake --build build-asan
```

```bash
gcc -std=c17 -Wall -Wextra -c intarray.c -o intarray.o
ar rcs libintarray.a intarray.o          # r=insert c=create s=index

gcc main.c -L. -lintarray -o app         # -L where, -l which
./app                                    # no runtime dependency
```

```bash
gcc -std=c17 -Wall -Wextra -fPIC -c intarray.c -o intarray.o
gcc -shared intarray.o -o libintarray.so

gcc main.c -L. -lintarray -o app
LD_LIBRARY_PATH=. ./app                  # must be findable at run time
```

```bash
gcc -lintarray main.c -o app     # FAILS
gcc main.c -lintarray -o app     # works
```

```bash
nm intarray.o
```
