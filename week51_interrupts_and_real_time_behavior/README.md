# Week 51 — Interrupts and Real-Time Behavior

📖 **[Lecture notes](https://sercankulcu.github.io/files/c/Week_51_Interrupts_and_Real_Time_Behavior.html)** — read these first; the code here is the worked example from that page.

> **Platform:** cross-compiled for ARM Cortex-M and run under QEMU. No hardware needed.
> Install `gcc-arm-none-eabi` and `qemu-system-arm`.

## Files

- `interrupts.c`

## Build and run

```bash
make
make run
```

## Experiments from the notes

These are the commands the lecture notes ask you to run — including the ones that
are supposed to fail.

```bash
arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb \
    -ffreestanding -nostdlib -O2 -Wall -Wextra -g \
    -T firmware.ld -o firmware.elf startup.c interrupts.c

qemu-system-arm -M lm3s6965evb -nographic -kernel firmware.elf
```
