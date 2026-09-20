# Week 55 — Embedded Debugging and Testing

📖 **[Lecture notes](https://sercankulcu.github.io/files/c/Week_55_Embedded_Debugging_and_Testing.html)** — read these first; the code here is the worked example from that page.

> **Platform:** cross-compiled for ARM Cortex-M and run under QEMU. No hardware needed.
> Install `gcc-arm-none-eabi` and `qemu-system-arm`.

## Files

- `fuzz_protocol.c`
- `hal.h`
- `hal_device.c`
- `protocol.c`
- `protocol.h`
- `test_protocol.c`

## Build and run

```bash
make
make run
```

## Experiments from the notes

These are the commands the lecture notes ask you to run — including the ones that
are supposed to fail.

```bash
gdb-multiarch firmware.elf
(gdb) target remote :1234        # or :3333 for OpenOCD
(gdb) load                       # program the flash (hardware only)
(gdb) monitor reset halt
(gdb) break main
(gdb) continue
```

```bash
gcc -std=c17 -Wall -Wextra -g -fsanitize=address,undefined \
    -o test_protocol test_protocol.c protocol.c
./test_protocol ; echo "status $?"
```

```bash
clang -g -O1 -fsanitize=fuzzer,address,undefined \
      -o fuzz_protocol fuzz_protocol.c protocol.c
./fuzz_protocol corpus/
```

```bash
qemu-system-arm -M lm3s6965evb -nographic -kernel firmware.elf -S -s &
gdb-multiarch firmware.elf
(gdb) target remote :1234
(gdb) break protocol_poll
(gdb) continue
(gdb) print *p
(gdb) print p->line
(gdb) watch p->used             # who changes it, and when
(gdb) print/x *(UART_Type *)0x4000C000
```
