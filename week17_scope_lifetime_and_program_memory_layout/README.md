# Week 17 — Scope, Lifetime, and Program Memory Layout

📖 **[Lecture notes](https://sercankulcu.github.io/files/c/Week_17_Scope_and_Program_Memory_Layout.html)** — read these first; the code here is the worked example from that page.

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
gcc -Wshadow ..
```

```bash
gcc -std=c17 -Wall -Wextra -Wshadow -g -o layout layout.c
./layout
```

```bash
size layout
```

```bash
nm layout | grep -E ' [BDRT] ' | sort
```
