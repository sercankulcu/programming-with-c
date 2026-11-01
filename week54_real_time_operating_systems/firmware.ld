ENTRY(reset_handler)                 /* the entry symbol, for debuggers */

MEMORY
{
    FLASH (rx)  : ORIGIN = 0x00000000, LENGTH = 256K
    RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 64K
}

/* The main stack. Under FreeRTOS it is used only by main() before the
   scheduler starts and by interrupt handlers afterwards; every task gets
   its own stack from the FreeRTOS heap. */
_stack_size = 4K;

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
        *(.bss*)                     /* includes ucHeap, configTOTAL_HEAP_SIZE */
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

    /* Fail the build rather than the device - a heap too big for RAM
       stops here. */
    ASSERT(_stack_top <= ORIGIN(RAM) + LENGTH(RAM), "RAM overflow")
}
