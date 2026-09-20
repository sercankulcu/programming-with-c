# Week 46 — ABI and Interfacing with Other Languages

📖 **[Lecture notes](https://sercankulcu.github.io/files/c/Week_46_ABI_and_Language_Interoperability.html)** — read these first; the code here is the worked example from that page.

## Files

- `app.cpp`
- `app.py`
- `main.rs`
- `mathlib.c`
- `mathlib.h`

## Build and run

```bash
make
make run
```

## Experiments from the notes

These are the commands the lecture notes ask you to run — including the ones that
are supposed to fail.

```bash
gcc -O2 -S -masm=intel example.c -o example.s
```

```bash
nm -D libfoo.so | c++filt        # decode mangled names
```

```bash
gcc -std=c17 -Wall -Wextra -O2 -fPIC -shared -o libmathlib.so mathlib.c
```

```bash
python3 app.py
```

```bash
rustc -L . main.rs -o rustapp
LD_LIBRARY_PATH=. ./rustapp
```

```bash
g++ -std=c++17 app.cpp -L. -lmathlib -o cppapp
LD_LIBRARY_PATH=. ./cppapp
```
