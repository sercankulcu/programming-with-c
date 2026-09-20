# Week 24 — The Standard Library Toolbox

📖 **[Lecture notes](https://sercankulcu.github.io/files/c/Week_24_Standard_Library_Toolbox.html)** — read these first; the code here is the worked example from that page.

## Files

- `toolbox.c`

## Build and run

```bash
make
make run
```

## Experiments from the notes

These are the commands the lecture notes ask you to run — including the ones that
are supposed to fail.

```bash
gcc -std=c17 -Wall -Wextra prog.c -o prog -lm
```

```bash
clock_t start = clock();
do_work();
double cpu_seconds = (double)(clock() - start) / CLOCKS_PER_SEC;
```

```bash
gcc -std=c17 -Wall -Wextra -g -o toolbox toolbox.c -lm
./toolbox
```

```bash
gcc -std=c17 -Wall -Wextra -o toolbox toolbox.c
```
