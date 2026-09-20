# Week 49 — Embedded C and Cross-Compilation

📖 **[Lecture notes](https://sercankulcu.github.io/files/c/Week_49_Embedded_C_and_Cross_Compilation.html)** — read these first; the code here is the worked example from that page.

> **Platform:** cross-compiled for ARM Cortex-M and run under QEMU. No hardware needed.
> Install `gcc-arm-none-eabi` and `qemu-system-arm`.

## Files

- `firmware.ld`
- `main.c`
- `startup.c`

## Build and run

```bash
make
make run
```

## Experiments from the notes

These are the commands the lecture notes ask you to run — including the ones that
are supposed to fail.

```bash
arm-none-eabi-gcc
│   │    │
│   │    └── ABI: embedded ABI
│   └─────── vendor/OS: none — bare metal
└─────────── architecture: ARM

x86_64-linux-gnu-gcc        # the compiler you have been using
riscv64-unknown-elf-gcc     # bare-metal RISC-V
```

```bash
arm-none-eabi-gcc -ffreestanding -nostdlib ..
```

```bash
arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb \
    -ffreestanding -nostdlib -O2 -Wall -Wextra -g \
    -T firmware.ld -o firmware.elf startup.c main.c

arm-none-eabi-objcopy -O binary firmware.elf firmware.bin
arm-none-eabi-size firmware.elf

qemu-system-arm -M lm3s6965evb -nographic -kernel firmware.elf
```

```bash
arm-none-eabi-size firmware.elf
```

```bash
arm-none-eabi-objdump -h firmware.elf          # section addresses
arm-none-eabi-objdump -d firmware.elf | head -30
arm-none-eabi-nm -n firmware.elf | head -20
xxd -l 16 firmware.bin                          # the vector table
```
