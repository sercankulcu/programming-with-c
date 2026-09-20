# Week 19 — Dynamic Memory Allocation

📖 **[Lecture notes](https://sercankulcu.github.io/files/c/Week_19_Dynamic_Memory_Allocation.html)** — read these first; the code here is the worked example from that page.

## Files

- `dynarray.c`

## Build and run

```bash
make
make run
```

## Experiments from the notes

These are the commands the lecture notes ask you to run — including the ones that
are supposed to fail.

```bash
gcc -std=c17 -Wall -Wextra -g -fsanitize=address -o dynarray dynarray.c
./dynarray
```

```bash
./dynarray
(no LeakSanitizer output = nothing leaked)
```

```bash
valgrind --leak-check=full ./dynarray
```
