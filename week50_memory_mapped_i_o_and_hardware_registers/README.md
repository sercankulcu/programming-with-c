# Week 50 — Memory-Mapped I/O and Hardware Registers

📖 **[Lecture notes](https://sercankulcu.github.io/files/c/Week_50_Memory_Mapped_IO_and_Registers.html)** — read these first; the code here is the worked example from that page.

> **Platform:** cross-compiled for ARM Cortex-M and run under QEMU. No hardware needed.
> Install `gcc-arm-none-eabi` and `qemu-system-arm`.

## Files

- `main.c`
- `uart.c`

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
    -T firmware.ld -o firmware.elf startup.c uart.c main.c

qemu-system-arm -M lm3s6965evb -nographic -kernel firmware.elf
```

```bash
arm-none-eabi-objdump -d firmware.elf \
    | sed -n '/<demonstrate_without_volatile>:/,/^$/p'
```

```bash
arm-none-eabi-gcc .. -O0 .. && qemu-system-arm ..   # usually still works
arm-none-eabi-gcc .. -O2 .. && qemu-system-arm ..   # hangs, or garbles output
```
