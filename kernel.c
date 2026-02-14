// kernel.c - Famantic OS Main Kernel
#include "kernel.h"

void kernel_main(void) {
    // Initialize terminal
    terminal_initialize();
    
    // Initialize GDT
    gdt_install();
    
    // Initialize IDT
    idt_install();
    
    // Initialize ISRs
    isrs_install();
    
    // Initialize IRQs
    irq_install();
    
    // Enable interrupts
    __asm__ __volatile__ ("sti");
    
    // Initialize keyboard
    keyboard_install();
    
    // Display welcome message
    terminal_setcolor(VGA_COLOR_LIGHT_CYAN);
    terminal_writestring("================================================================================\n");
    terminal_setcolor(VGA_COLOR_LIGHT_GREEN);
    terminal_writestring("                         FAMANTIC OS v1.0.0 Alpha\n");
    terminal_setcolor(VGA_COLOR_LIGHT_CYAN);
    terminal_writestring("================================================================================\n\n");
    
    terminal_setcolor(VGA_COLOR_WHITE);
    terminal_writestring("Welcome to Famantic OS - A Linux-like Operating System\n\n");
    
    terminal_setcolor(VGA_COLOR_LIGHT_GREEN);
    terminal_writestring("[OK] ");
    terminal_setcolor(VGA_COLOR_WHITE);
    terminal_writestring("Kernel loaded successfully\n");
    
    terminal_setcolor(VGA_COLOR_LIGHT_GREEN);
    terminal_writestring("[OK] ");
    terminal_setcolor(VGA_COLOR_WHITE);
    terminal_writestring("GDT initialized\n");
    
    terminal_setcolor(VGA_COLOR_LIGHT_GREEN);
    terminal_writestring("[OK] ");
    terminal_setcolor(VGA_COLOR_WHITE);
    terminal_writestring("IDT initialized\n");
    
    terminal_setcolor(VGA_COLOR_LIGHT_GREEN);
    terminal_writestring("[OK] ");
    terminal_setcolor(VGA_COLOR_WHITE);
    terminal_writestring("Interrupts enabled\n");
    
    terminal_setcolor(VGA_COLOR_LIGHT_GREEN);
    terminal_writestring("[OK] ");
    terminal_setcolor(VGA_COLOR_WHITE);
    terminal_writestring("Keyboard driver loaded\n\n");
    
    terminal_setcolor(VGA_COLOR_YELLOW);
    terminal_writestring("Type 'help' for available commands\n\n");
    
    terminal_setcolor(VGA_COLOR_WHITE);
    
    // Start shell
    shell_start();
    
    // Should never reach here
    terminal_setcolor(VGA_COLOR_LIGHT_RED);
    terminal_writestring("\nKernel panic: Shell terminated unexpectedly!\n");
    for(;;);
}
