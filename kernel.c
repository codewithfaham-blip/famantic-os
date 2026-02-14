#include "kheap.h"

extern char _kernel_start[];
extern char _kernel_end[];

struct multiboot_info* global_mbi;

void kernel_main(uint32_t magic, uint32_t mb_info_addr) {
    // Initialize terminal
    terminal_initialize();
    
    global_mbi = (struct multiboot_info*)mb_info_addr;
    
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
    
    // Initialize PMM
    uint32_t mem_size = 128 * 1024 * 1024; // Default if not found
    if (global_mbi->flags & (1 << 0)) {
        mem_size = (global_mbi->mem_lower + global_mbi->mem_upper) * 1024;
    }
    
    // Bitmap after kernel
    uint32_t bitmap_start = (uint32_t)_kernel_end;
    uint32_t bitmap_size = mem_size / PAGE_SIZE / 8;
    pmm_init(mem_size, bitmap_start);
    
    // Mark free regions from multiboot mmap
    if (global_mbi->flags & (1 << 6)) {
        struct multiboot_mmap_entry* mmap = (struct multiboot_mmap_entry*)global_mbi->mmap_addr;
        while ((uint32_t)mmap < global_mbi->mmap_addr + global_mbi->mmap_length) {
            if (mmap->type == 1) { // Available
                uint32_t addr = mmap->addr_low;
                uint32_t len = mmap->len_low;
                for (uint32_t j = 0; j < len; j += PAGE_SIZE) {
                    pmm_mark_free(addr + j);
                }
            }
            mmap = (struct multiboot_mmap_entry*)((uint32_t)mmap + mmap->size + sizeof(mmap->size));
        }
    } else {
        // Fallback: free some memory
        for (uint32_t i = 0x100000; i < mem_size; i += PAGE_SIZE) {
            pmm_mark_free(i);
        }
    }
    
    // Re-mark kernel and bitmap as used
    for (uint32_t i = (uint32_t)_kernel_start; i < (uint32_t)_kernel_end + bitmap_size; i += PAGE_SIZE) {
        pmm_mark_used(i);
    }
    // Reserve first 1MB
    for (uint32_t i = 0; i < 0x100000; i += PAGE_SIZE) {
        pmm_mark_used(i);
    }

    // Initialize Heap
    uint32_t heap_start = bitmap_start + bitmap_size + 4096;
    uint32_t heap_size = 4 * 1024 * 1024; // 4MB
    kheap_init(heap_start, heap_size);
    for (uint32_t i = heap_start; i < heap_start + heap_size; i += PAGE_SIZE) {
        pmm_mark_used(i);
    }

    // Display welcome message
    terminal_setcolor(VGA_COLOR_LIGHT_CYAN);
    terminal_writestring("================================================================================\n");
    terminal_setcolor(VGA_COLOR_LIGHT_GREEN);
    terminal_writestring("                         FAMANTIC OS v1.2.0 Advance\n");
    terminal_setcolor(VGA_COLOR_LIGHT_CYAN);
    terminal_writestring("================================================================================\n\n");
    
    terminal_setcolor(VGA_COLOR_WHITE);
    terminal_writestring("Welcome to Famantic OS - Enhanced Advanced Kernel\n\n");
    
    printf("[OK] Kernel: %p - %p\n", _kernel_start, _kernel_end);
    printf("[OK] Memory: %i MB total, %i blocks free\n", mem_size / (1024 * 1024), pmm_get_free_blocks());
    printf("[OK] Heap: %i KB total at %p\n", heap_size / 1024, heap_start);
    terminal_setcolor(VGA_COLOR_LIGHT_GREEN);
    terminal_writestring("[OK] ");
    terminal_setcolor(VGA_COLOR_WHITE);
    terminal_writestring("PMM and Heap initialized\n");
    
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
