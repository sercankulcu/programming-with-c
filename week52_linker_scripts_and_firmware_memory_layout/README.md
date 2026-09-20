# Week 52 — Linker Scripts and Firmware Memory Layout

📖 **[Lecture notes](https://sercankulcu.github.io/files/c/Week_52_Linker_Scripts_and_Firmware_Layout.html)** — read these first; the code here is the worked example from that page.

> **Platform:** cross-compiled for ARM Cortex-M and run under QEMU. No hardware needed.
> Install `gcc-arm-none-eabi` and `qemu-system-arm`.

## Files

- `layout.c`

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
    -T firmware.ld -Wl,-Map=firmware.map \
    -o firmware.elf startup.c layout.c

qemu-system-arm -M lm3s6965evb -nographic -kernel firmware.elf
```

```bash
arm-none-eabi-size firmware.elf
```

```bash
arm-none-eabi-objdump -h firmware.elf
```

```bash
grep -A20 'Memory Configuration' firmware.map
grep -E '^\s+\.(text|data|bss)' firmware.map | head -20
grep -B2 -A8 '_stack_top' firmware.map
```

```bash
arm-none-eabi-nm --size-sort -S firmware.elf | tail -10   # the biggest symbols
```

```bash
arm-none-eabi-ld: RAM overflow
collect2: error: ld returned 1 exit status
```
