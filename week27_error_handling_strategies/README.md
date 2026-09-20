# Week 27 — Error Handling Strategies

📖 **[Lecture notes](https://sercankulcu.github.io/files/c/Week_27_Error_Handling.html)** — read these first; the code here is the worked example from that page.

## Files

- `csv.h`
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
gcc -DNDEBUG -O2 ..          # assertions compiled out
```

```bash
gcc -std=c17 -Wall -Wextra -g -fsanitize=address -o errors errors.c
./errors
```
