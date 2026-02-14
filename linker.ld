/* Famantic OS Linker Script */
/* This script tells the linker how to lay out the kernel in memory */

ENTRY(_start)

SECTIONS
{
    /* Kernel starts at 1MB in physical memory */
    /* This is the standard location for x86 kernels */
    . = 1M;

    /* Multiboot header must be at the very beginning */
    .text ALIGN(4K) : AT(ADDR(.text))
    {
        *(.multiboot)
        *(.text)
    }

    /* Read-only data section */
    .rodata ALIGN(4K) : AT(ADDR(.rodata))
    {
        *(.rodata)
        *(.rodata.*)
    }

    /* Initialized data section */
    .data ALIGN(4K) : AT(ADDR(.data))
    {
        *(.data)
        *(.data.*)
    }

    /* Uninitialized data section (BSS) */
    .bss ALIGN(4K) : AT(ADDR(.bss))
    {
        *(COMMON)
        *(.bss)
        *(.bss.*)
    }

    /* Discard unnecessary sections */
    /DISCARD/ :
    {
        *(.comment)
        *(.eh_frame)
        *(.note.*)
    }
}
