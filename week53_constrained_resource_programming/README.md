# Week 53 — Constrained-Resource Programming

📖 **[Lecture notes](https://sercankulcu.github.io/files/c/Week_53_Constrained_Resource_Programming.html)** — read these first; the code here is the worked example from that page.

> **Platform:** cross-compiled for ARM Cortex-M and run under QEMU. No hardware needed.
> Install `gcc-arm-none-eabi` and `qemu-system-arm`.

## Files

- `constrained.c`

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
    -ffreestanding -nostdlib -Os -Wall -Wextra -g \
    -ffunction-sections -fdata-sections -Wl,--gc-sections \
    -T firmware.ld -Wl,-Map=firmware.map \
    -o firmware.elf startup.c constrained.c

arm-none-eabi-size firmware.elf
qemu-system-arm -M lm3s6965evb -nographic -kernel firmware.elf
```

```bash
arm-none-eabi-size firmware.elf
arm-none-eabi-nm --size-sort -S firmware.elf | tail -15
```
