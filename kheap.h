#ifndef KHEAP_H
#define KHEAP_H

#include <stdint.h>
#include <stddef.h>

void kheap_init(uint32_t start, uint32_t size);
void* kmalloc(size_t size);
void kfree(void* ptr);

uint32_t kheap_get_used();
uint32_t kheap_get_free();

#endif
