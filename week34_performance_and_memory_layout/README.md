# Week 34 — Performance and Memory Layout

📖 **[Lecture notes](https://sercankulcu.github.io/files/c/Week_34_Performance_and_Memory_Layout.html)** — read these first; the code here is the worked example from that page.

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
gcc -O2 -S -masm=intel prog.c -o prog.s     # generate assembly
objdump -d --demangle prog.o | less          # disassemble an object file
```

```bash
gcc -std=c17 -Wall -Wextra -O2 -o perf perf.c
./perf
```

```bash
perf stat -e cache-misses,cache-references,instructions,cycles ./perf
```

```bash
gcc -O2 -S perf.c -o perf.s
sed -n '/^sum_to:/,/ret/p' perf.s
```

```bash
gcc -O0 -S perf.c -o perf0.s
sed -n '/^sum_to:/,/ret/p' perf0.s      # a literal loop, everything in memory
```

```bash
perf record -g ./perf
perf report --stdio | head -20
```
