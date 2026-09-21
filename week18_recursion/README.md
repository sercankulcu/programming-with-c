# Week 18 — Recursion

📖 **[Lecture notes](https://sercankulcu.github.io/files/c/Week_18_Recursion.html)** — read these first; the code here is the worked example from that page.

## Files

- `recur.c`
- `recur.c`

## Build and run

```bash
make
make run
```

## Experiments from the notes

These are the commands the lecture notes ask you to run — including the ones that
are supposed to fail.

```bash
gcc -fsanitize=address ..
==1234==ERROR: AddressSanitizer: stack-overflow on address 0x7ffc...
```

```bash
gcc -std=c17 -Wall -Wextra -g -o recur recur.c
./recur
```

```bash
gdb ./recur
(gdb) break factorial if n == 1
(gdb) run
(gdb) backtrace
```

```bash
gcc -std=c17 -Wall -Wextra -g -fsanitize=address -o recur_asan recur.c
./recur_asan
```
