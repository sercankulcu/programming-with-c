# Week 23 — Program Arguments and Input

📖 **[Lecture notes](https://sercankulcu.github.io/files/c/Week_23_Program_Arguments_and_Input.html)** — read these first; the code here is the worked example from that page.

## Files

- `scale.c`
- `variant2.c`

## Build and run

```bash
make
make run
```

## Experiments from the notes

These are the commands the lecture notes ask you to run — including the ones that
are supposed to fail.

```bash
$ ./prog hello 42 "two words"
argv[0] = "./prog"
argv[1] = "hello"
argv[2] = "42"
argv[3] = "two words"
```

```bash
gcc -std=c17 -Wall -Wextra -g -fsanitize=address -o scale scale.c
```

```bash
python3 -c "print('1' * 300)" | ./scale 1 0
```
