# Week 02 — History, Standards, and the Compilation Model

📖 **[Lecture notes](https://sercankulcu.github.io/files/c/Week_02_History_Standards_and_Compilation.html)** — read these first; the code here is the worked example from that page.

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
gcc -E hello.c -o hello.i
wc -l hello.i
```

```bash
gcc -S hello.c -o hello.s
cat hello.s
```

```bash
gcc -c hello.c -o hello.o
file hello.o
nm hello.o
```

```bash
gcc hello.o -o hello
./hello
```

```bash
gcc -std=c89 -Wall -Wextra -o era era.c
```

```bash
gcc -std=c23 -Wall -Wextra -o era era.c
./era
```
