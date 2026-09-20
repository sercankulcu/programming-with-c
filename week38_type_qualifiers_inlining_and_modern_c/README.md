# Week 38 — Type Qualifiers, Inlining, and Modern C

📖 **[Lecture notes](https://sercankulcu.github.io/files/c/Week_38_Type_Qualifiers_and_Modern_C.html)** — read these first; the code here is the worked example from that page.

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
gcc -std=c17 -Wall -Wextra -O2 -o modern modern.c
./modern
```

```bash
gcc -O2 -S modern.c -o modern.s
sed -n '/^add_plain:/,/ret/p'    modern.s | grep -c 'xmm\|ymm'
sed -n '/^add_restrict:/,/ret/p' modern.s | grep -c 'xmm\|ymm'
```

```bash
gcc -O2 -S modern.c -o modern.s
grep -B3 -A6 'observed' modern.s
```
