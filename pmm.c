#include "pmm.h"
#include "kernel.h"

static uint32_t* pmm_bitmap;
static uint32_t pmm_max_blocks;
static uint32_t pmm_used_blocks;

void pmm_init(uint32_t mem_size, uint32_t bitmap_start) {
    pmm_max_blocks = mem_size / PAGE_SIZE;
    pmm_bitmap = (uint32_t*)bitmap_start;
    pmm_used_blocks = pmm_max_blocks;
    
    // Default all memory to used
    memset(pmm_bitmap, 0xFF, pmm_max_blocks / 8);
}

void pmm_mark_used(uint32_t addr) {
    uint32_t block = addr / PAGE_SIZE;
    if (!(pmm_bitmap[block / 32] & (1 << (block % 32)))) {
        pmm_bitmap[block / 32] |= (1 << (block % 32));
        pmm_used_blocks++;
    }
}

void pmm_mark_free(uint32_t addr) {
    uint32_t block = addr / PAGE_SIZE;
    if (pmm_bitmap[block / 32] & (1 << (block % 32))) {
        pmm_bitmap[block / 32] &= ~(1 << (block % 32));
        pmm_used_blocks--;
    }
}

void* pmm_alloc_block() {
    for (uint32_t i = 0; i < pmm_max_blocks / 32; i++) {
        if (pmm_bitmap[i] != 0xFFFFFFFF) {
            for (uint32_t j = 0; j < 32; j++) {
                if (!(pmm_bitmap[i] & (1 << j))) {
                    uint32_t block = i * 32 + j;
                    uint32_t addr = block * PAGE_SIZE;
                    pmm_bitmap[block / 32] |= (1 << (block % 32));
                    pmm_used_blocks++;
                    return (void*)addr;
                }
            }
        }
    }
    return NULL;
}

void pmm_free_block(void* addr) {
    uint32_t block = (uint32_t)addr / PAGE_SIZE;
    if (pmm_bitmap[block / 32] & (1 << (block % 32))) {
        pmm_bitmap[block / 32] &= ~(1 << (block % 32));
        pmm_used_blocks--;
    }
}

uint32_t pmm_get_used_blocks() {
    return pmm_used_blocks;
}

uint32_t pmm_get_free_blocks() {
    return pmm_max_blocks - pmm_used_blocks;
}
