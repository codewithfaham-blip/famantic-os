#ifndef PMM_H
#define PMM_H

#include <stdint.h>
#include <stddef.h>

#define PAGE_SIZE 4096

void pmm_init(uint32_t mem_size, uint32_t bitmap_start);
void pmm_mark_used(uint32_t addr);
void pmm_mark_free(uint32_t addr);
void* pmm_alloc_block();
void pmm_free_block(void* addr);

uint32_t pmm_get_used_blocks();
uint32_t pmm_get_free_blocks();

#endif
