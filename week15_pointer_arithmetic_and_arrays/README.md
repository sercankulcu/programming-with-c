# Week 15 — Pointer Arithmetic and Arrays

📖 **[Lecture notes](https://sercankulcu.github.io/files/c/Week_15_Pointer_Arithmetic_and_Arrays.html)** — read these first; the code here is the worked example from that page.

## Files

- `ptrmath.c`

## Build and run

```bash
make
make run
```

## Experiments from the notes

These are the commands the lecture notes ask you to run — including the ones that
are supposed to fail.

```bash
gcc -std=c17 -Wall -Wextra -g -fsanitize=address -o ptrmath ptrmath.c
./ptrmath
```

```bash
gcc -std=c17 -O2 -S ptrmath.c -o ptrmath.s
sed -n '/sum_indexed:/,/ret/p' ptrmath.s
sed -n '/sum_pointer:/,/ret/p' ptrmath.s
```
