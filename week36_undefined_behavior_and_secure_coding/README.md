# Week 36 — Undefined Behavior and Secure Coding

📖 **[Lecture notes](https://sercankulcu.github.io/files/c/Week_36_Undefined_Behavior_and_Secure_Coding.html)** — read these first; the code here is the worked example from that page.

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
gcc -std=c17 -Wall -Wextra -Wformat-security -O0 -o ub0 ub.c
gcc -std=c17 -Wall -Wextra -Wformat-security -O2 -o ub2 ub.c
./ub0 > out0.txt
./ub2 > out2.txt
diff out0.txt out2.txt
```

```bash
gcc -std=c17 -Wall -Wextra -g -fsanitize=undefined -o ubsan ub.c
./ubsan
```

```bash
gcc -O2 -S ub.c -o ub.s
sed -n '/bits_memcpy:/,/ret/p' ub.s
sed -n '/bits_violating:/,/ret/p' ub.s
```
