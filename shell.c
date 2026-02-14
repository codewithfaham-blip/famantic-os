// shell.c - Simple shell implementation
#include "kernel.h"

#define MAX_COMMAND_LENGTH 256

void shell_prompt(void) {
    terminal_setcolor(VGA_COLOR_LIGHT_GREEN);
    terminal_writestring("famantic");
    terminal_setcolor(VGA_COLOR_WHITE);
    terminal_writestring("@");
    terminal_setcolor(VGA_COLOR_LIGHT_BLUE);
    terminal_writestring("localhost");
    terminal_setcolor(VGA_COLOR_WHITE);
    terminal_writestring(":~$ ");
}

void cmd_help(void) {
    terminal_setcolor(VGA_COLOR_YELLOW);
    terminal_writestring("Available commands:\n");
    terminal_setcolor(VGA_COLOR_WHITE);
    terminal_writestring("  help     - Show this help message\n");
    terminal_writestring("  clear    - Clear the screen\n");
    terminal_writestring("  echo     - Echo text to screen\n");
    terminal_writestring("  about    - About Famantic OS\n");
    terminal_writestring("  version  - Show OS version\n");
    terminal_writestring("  meminfo  - Show memory information\n");
    terminal_writestring("  cpuid    - Show CPU information\n");
    terminal_writestring("  pmm      - Show physical memory status\n");
    terminal_writestring("  heap     - Show kernel heap status\n");
    terminal_writestring("  reboot   - Reboot the system\n");
    terminal_writestring("  shutdown - Shutdown the system\n\n");
}

void cmd_clear(void) {
    terminal_clear();
}

void cmd_echo(const char* args) {
    if (args && *args) {
        terminal_writestring(args);
        terminal_putchar('\n');
    } else {
        terminal_writestring("\n");
    }
}

void cmd_about(void) {
    terminal_setcolor(VGA_COLOR_LIGHT_CYAN);
    terminal_writestring("\n");
    terminal_writestring("Famantic OS - A Linux-like Operating System\n");
    terminal_writestring("========================================\n\n");
    terminal_setcolor(VGA_COLOR_WHITE);
    terminal_writestring("Famantic OS is a simple operating system written in C and Assembly.\n");
    terminal_writestring("It features:\n");
    terminal_writestring("  * Custom bootloader with GRUB support\n");
    terminal_writestring("  * VGA text mode driver\n");
    terminal_writestring("  * Keyboard input handling\n");
    terminal_writestring("  * Basic shell with commands\n");
    terminal_writestring("  * Interrupt handling (IDT/IRQ)\n");
    terminal_writestring("  * Memory segmentation (GDT)\n\n");
    terminal_setcolor(VGA_COLOR_YELLOW);
    terminal_writestring("Created for educational purposes!\n\n");
    terminal_setcolor(VGA_COLOR_WHITE);
}

void cmd_version(void) {
    terminal_setcolor(VGA_COLOR_LIGHT_GREEN);
    terminal_writestring("Famantic OS v1.0.0 Alpha\n");
    terminal_setcolor(VGA_COLOR_WHITE);
    terminal_writestring("Build date: 2026-02-14\n");
    terminal_writestring("Kernel: Famantic-x86\n\n");
}

void cmd_reboot(void) {
    terminal_setcolor(VGA_COLOR_YELLOW);
    terminal_writestring("Rebooting system...\n");
    
    // Wait a moment
    for (volatile int i = 0; i < 10000000; i++);
    
    // Trigger keyboard controller reset
    uint8_t temp;
    __asm__ __volatile__ ("cli");
    
    do {
        temp = inb(0x64);
        if (temp & 1)
            inb(0x60);
    } while (temp & 2);
    
    outb(0x64, 0xFE);
    
    // If that didn't work, hang
    __asm__ __volatile__ ("hlt");
}

void cmd_shutdown(void) {
    terminal_setcolor(VGA_COLOR_YELLOW);
    terminal_writestring("Shutting down system...\n");
    terminal_writestring("You can now safely turn off your computer.\n");
    
    // Attempt ACPI shutdown
    outb(0xB004, 0x2000); // QEMU
    outb(0x604, 0x2000);  // Bochs
    outb(0x4004, 0x3400); // VirtualBox
    
    // If still running, halt
    __asm__ __volatile__ ("cli; hlt");
}

#include "multiboot.h"

extern struct multiboot_info* global_mbi;

void cmd_meminfo(void) {
    if (!global_mbi) {
        terminal_setcolor(VGA_COLOR_LIGHT_RED);
        terminal_writestring("Multiboot info not available!\n");
        return;
    }

    terminal_setcolor(VGA_COLOR_LIGHT_CYAN);
    terminal_writestring("Memory Information:\n");
    terminal_setcolor(VGA_COLOR_WHITE);
    
    printf("Lower memory: %d KB\n", global_mbi->mem_lower);
    printf("Upper memory: %d KB\n", global_mbi->mem_upper);
    printf("Total memory: %d MB\n", (global_mbi->mem_lower + global_mbi->mem_upper) / 1024);

    if (global_mbi->flags & (1 << 6)) {
        terminal_writestring("\nMemory Map:\n");
        struct multiboot_mmap_entry* mmap = (struct multiboot_mmap_entry*)global_mbi->mmap_addr;
        while ((uint32_t)mmap < global_mbi->mmap_addr + global_mbi->mmap_length) {
            printf("  Addr: %x, Len: %x, Type: %d\n", mmap->addr_low, mmap->len_low, mmap->type);
            mmap = (struct multiboot_mmap_entry*)((uint32_t)mmap + mmap->size + sizeof(mmap->size));
        }
    }
}

void cmd_cpuid(void) {
    uint32_t eax, ebx, ecx, edx;
    
    // CPU Vendor String
    char vendor[13];
    __asm__ __volatile__ ("cpuid" : "=b"(ebx), "=c"(ecx), "=d"(edx) : "a"(0));
    ((uint32_t*)vendor)[0] = ebx;
    ((uint32_t*)vendor)[1] = edx;
    ((uint32_t*)vendor)[2] = ecx;
    vendor[12] = '\0';
    
    terminal_setcolor(VGA_COLOR_LIGHT_CYAN);
    printf("CPU Vendor: %s\n", vendor);
    
    // Features
    __asm__ __volatile__ ("cpuid" : "=a"(eax), "=d"(edx) : "a"(1));
    terminal_setcolor(VGA_COLOR_WHITE);
    printf("Model: %i, Family: %i\n", (eax >> 4) & 0xF, (eax >> 8) & 0xF);
    
    terminal_writestring("Features: ");
    if (edx & (1 << 0)) terminal_writestring("FPU ");
    if (edx & (1 << 23)) terminal_writestring("MMX ");
    if (edx & (1 << 25)) terminal_writestring("SSE ");
    if (edx & (1 << 26)) terminal_writestring("SSE2 ");
    terminal_writestring("\n");
}

#include "pmm.h"

void cmd_pmm(void) {
    terminal_setcolor(VGA_COLOR_LIGHT_CYAN);
    terminal_writestring("Physical Memory Manager Status:\n");
    terminal_setcolor(VGA_COLOR_WHITE);
    printf("  Used Blocks: %d (%d KB)\n", pmm_get_used_blocks(), pmm_get_used_blocks() * 4);
    printf("  Free Blocks: %d (%d KB)\n", pmm_get_free_blocks(), pmm_get_free_blocks() * 4);
}

#include "kheap.h"

void cmd_heap_info(void) {
    terminal_setcolor(VGA_COLOR_LIGHT_CYAN);
    terminal_writestring("Kernel Heap Status:\n");
    terminal_setcolor(VGA_COLOR_WHITE);
    printf("  Used: %d bytes\n", kheap_get_used());
    printf("  Free: %d bytes\n", kheap_get_free());
}

void shell_execute(const char* command) {
    // Skip leading spaces
    while (*command == ' ')
        command++;
    
    if (*command == '\0')
        return;
    
    // Find command end
    const char* space = command;
    while (*space && *space != ' ')
        space++;
    
    size_t cmd_len = space - command;
    const char* args = (*space) ? space + 1 : "";
    
    // Skip spaces in args
    while (*args == ' ')
        args++;
    
    if (strncmp(command, "help", cmd_len) == 0 && cmd_len == 4) {
        cmd_help();
    } else if (strncmp(command, "clear", cmd_len) == 0 && cmd_len == 5) {
        cmd_clear();
    } else if (strncmp(command, "echo", cmd_len) == 0 && cmd_len == 4) {
        cmd_echo(args);
    } else if (strncmp(command, "about", cmd_len) == 0 && cmd_len == 5) {
        cmd_about();
    } else if (strncmp(command, "version", cmd_len) == 0 && cmd_len == 7) {
        cmd_version();
    } else if (strncmp(command, "reboot", cmd_len) == 0 && cmd_len == 6) {
        cmd_reboot();
    } else if (strncmp(command, "shutdown", cmd_len) == 0 && cmd_len == 8) {
        cmd_shutdown();
    } else if (strncmp(command, "meminfo", cmd_len) == 0 && cmd_len == 7) {
        cmd_meminfo();
    } else if (strncmp(command, "cpuid", cmd_len) == 0 && cmd_len == 5) {
        cmd_cpuid();
    } else if (strncmp(command, "pmm", cmd_len) == 0 && cmd_len == 3) {
        cmd_pmm();
    } else if (strncmp(command, "heap", cmd_len) == 0 && cmd_len == 4) {
        cmd_heap_info();
    } else {
        terminal_setcolor(VGA_COLOR_LIGHT_RED);
        terminal_writestring("Command not found: ");
        terminal_write(command, cmd_len);
        terminal_writestring("\nType 'help' for available commands.\n");
        terminal_setcolor(VGA_COLOR_WHITE);
    }
}

void shell_start(void) {
    char command_buffer[MAX_COMMAND_LENGTH];
    
    while (1) {
        shell_prompt();
        keyboard_get_line(command_buffer, MAX_COMMAND_LENGTH);
        shell_execute(command_buffer);
    }
}
