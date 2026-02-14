// keyboard.c - Keyboard driver
#include "kernel.h"

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

static char key_buffer[256];
static int buffer_start = 0;
static int buffer_end = 0;

unsigned char keyboard_map[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
    '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    '-', 0, 0, 0, '+', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

void keyboard_handler(void) {
    unsigned char scancode = inb(KEYBOARD_DATA_PORT);
    
    if (scancode & 0x80) {
        // Key released
    } else {
        // Key pressed
        char key = keyboard_map[scancode];
        if (key) {
            key_buffer[buffer_end] = key;
            buffer_end = (buffer_end + 1) % 256;
        }
    }
    
    outb(0x20, 0x20); // Send EOI
}

void keyboard_install(void) {
    irq_install_handler(1, keyboard_handler);
}

char keyboard_getchar(void) {
    while (buffer_start == buffer_end) {
        // Wait for input
        __asm__ __volatile__("hlt");
    }
    
    char c = key_buffer[buffer_start];
    buffer_start = (buffer_start + 1) % 256;
    return c;
}

void keyboard_get_line(char* buffer, size_t max_len) {
    size_t i = 0;
    
    while (i < max_len - 1) {
        char c = keyboard_getchar();
        
        if (c == '\n') {
            terminal_putchar('\n');
            break;
        } else if (c == '\b') {
            if (i > 0) {
                i--;
                terminal_backspace();
            }
        } else if (c >= 32 && c <= 126) {
            buffer[i++] = c;
            terminal_putchar(c);
        }
    }
    
    buffer[i] = '\0';
}
