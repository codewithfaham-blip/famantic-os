// kernel.h - Main kernel header
#ifndef KERNEL_H
#define KERNEL_H

#include <stddef.h>
#include <stdint.h>

// VGA colors
enum vga_color {
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GREY = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_YELLOW = 14,
    VGA_COLOR_WHITE = 15,
};

// Terminal functions
void terminal_initialize(void);
void terminal_setcolor(uint8_t color);
void terminal_putchar(char c);
void terminal_write(const char* data, size_t size);
void terminal_writestring(const char* data);
void terminal_clear(void);
void terminal_backspace(void);

// String functions
size_t strlen(const char* str);
int strcmp(const char* s1, const char* s2);
int strncmp(const char* s1, const char* s2, size_t n);
char* strcpy(char* dest, const char* src);
void* memset(void* ptr, int value, size_t num);
void* memcpy(void* dest, const void* src, size_t n);

// Number conversion
void itoa(int num, char* str, int base);
void printf(const char* format, ...);

// Port I/O
void outb(uint16_t port, uint8_t value);
uint8_t inb(uint16_t port);

// GDT functions
void gdt_install(void);
extern void gdt_flush(void);

// IDT functions
void idt_install(void);
void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);
extern void idt_flush(void);

// ISR functions
void isrs_install(void);
void isr_handler(void);

// IRQ functions
void irq_install(void);
void irq_handler(void);
void irq_install_handler(int irq, void (*handler)(void));
void irq_uninstall_handler(int irq);

// Keyboard functions
void keyboard_install(void);
char keyboard_getchar(void);
void keyboard_get_line(char* buffer, size_t max_len);

// Shell functions
void shell_start(void);
void shell_execute(const char* command);

// Timer
void timer_install(void);
void timer_wait(int ticks);

#endif
