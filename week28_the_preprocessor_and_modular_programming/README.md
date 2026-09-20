# Week 28 — The Preprocessor and Modular Programming

📖 **[Lecture notes](https://sercankulcu.github.io/files/c/Week_28_Preprocessor_and_Modular_Programming.html)** — read these first; the code here is the worked example from that page.

## Files

- `buffer.c`
- `buffer.h`
- `config.h`
- `intarray.c`
- `intarray.h`
- `main.c`

## Build and run

```bash
make
make run
```

## Experiments from the notes

These are the commands the lecture notes ask you to run — including the ones that
are supposed to fail.

```bash
gcc -std=c17 -Wall -Wextra -g -fsanitize=address -c intarray.c -o intarray.o
gcc -std=c17 -Wall -Wextra -g -fsanitize=address -c main.c     -o main.o
gcc -fsanitize=address intarray.o main.o -o app
./app
```

```bash
gcc -E main.c | tail -40
```
