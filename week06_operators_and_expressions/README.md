# Week 06 — Operators and Expressions

📖 **[Lecture notes](https://sercankulcu.github.io/files/c/Week_06_Operators_and_Expressions.html)** — read these first; the code here is the worked example from that page.

## Files

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
gcc -std=c17 -Wall -Wextra -g -o expr expr.c
./expr
```

```bash
gcc -std=c17 -O0 -o expr0 expr.c && ./expr0 | tail -4
gcc -std=c17 -O2 -o expr2 expr.c && ./expr2 | tail -4
```
