# Week 12 — One- and Multi-Dimensional Arrays

📖 **[Lecture notes](https://sercankulcu.github.io/files/c/Week_12_Arrays.html)** — read these first; the code here is the worked example from that page.

## Files

- `arrays.c`

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
gcc -std=c17 -Wall -Wextra -g -o arrays arrays.c
./arrays
```

```bash
gcc -std=c17 -Wall -Wextra -g -o arrays arrays.c && ./arrays
```

```bash
gcc -std=c17 -Wall -Wextra -g -fsanitize=address -o arrays_asan arrays.c
./arrays_asan
```
