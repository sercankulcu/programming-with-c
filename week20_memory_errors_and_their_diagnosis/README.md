# Week 20 — Memory Errors and Their Diagnosis

📖 **[Lecture notes](https://sercankulcu.github.io/files/c/Week_20_Memory_Errors.html)** — read these first; the code here is the worked example from that page.

## Files

- `bugs.c`

## Build and run

```bash
make
make run
```

## Experiments from the notes

These are the commands the lecture notes ask you to run — including the ones that
are supposed to fail.

```bash
gcc -std=c17 -Wall -Wextra -g -fsanitize=address -o prog prog.c
./prog
```

```bash
valgrind --leak-check=full --track-origins=yes ./prog
```

```bash
gcc -std=c17 -Wall -Wextra -g -DBUG=2 -o bug2_plain bugs.c
valgrind --leak-check=full ./bug2_plain
```
