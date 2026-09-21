ENTRY(reset_handler)                 /* the entry symbol, for debuggers */

MEMORY
{
    FLASH (rx)  : ORIGIN = 0x00000000, LENGTH = 256K
    RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 64K
}

_stack_size = 4K;
_heap_size  = 0;                     /* no heap: week 53 */

SECTIONS
{
    .text : {
        KEEP(*(.vectors))            /* must be first, and never discarded */
        *(.text*)
        *(.rodata*)
        . = ALIGN(4);
        _etext = .;
    } > FLASH

    .data : {
        . = ALIGN(4);
        _data_start = .;
        *(.data*)
        . = ALIGN(4);
        _data_end = .;
    } > RAM AT> FLASH                /* VMA in RAM, LMA in FLASH */

    _data_load = LOADADDR(.data);    /* where the startup code reads from */

    .bss (NOLOAD) : {
        . = ALIGN(4);
        _bss_start = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        _bss_end = .;
    } > RAM

    .stack (NOLOAD) : {
        . = ALIGN(8);                /* ARM requires 8-byte stack alignment */
        _stack_bottom = .;
        . = . + _stack_size;
        _stack_top = .;
    } > RAM

    /* Fail the build rather than the device. */
    ASSERT(_stack_top <= ORIGIN(RAM) + LENGTH(RAM), "RAM overflow")
}
