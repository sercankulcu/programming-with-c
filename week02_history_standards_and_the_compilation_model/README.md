# Week 02 — History, Standards, and the Compilation Model

📖 **[Lecture notes](https://sercankulcu.github.io/files/c/Week_02_History_Standards_and_Compilation.html)** — read these first; the code here is the worked example from that page.

## Files

- `era.c`

## Build and run

```bash
make
make run
```

## Experiments from the notes

These are the commands the lecture notes ask you to run — including the ones that
are supposed to fail.

```bash
gcc -E era.c -o era.i
wc -l era.i
```

```bash
gcc -S era.c -o era.s
cat era.s
```

```bash
gcc -c era.c -o era.o
file era.o
nm era.o
```

```bash
gcc era.o -o hello
./hello
```

```bash
gcc -std=c89 -Wall -Wextra -o era era.c
```

```bash
gcc -std=c23 -Wall -Wextra -o era era.c
./era
```
