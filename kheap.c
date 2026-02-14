#include "kheap.h"
#include "kernel.h"

typedef struct header {
    size_t size;
    struct header* next;
    int is_free;
} header_t;

static header_t* head = NULL;
static uint32_t heap_used = 0;
static uint32_t heap_total = 0;

void kheap_init(uint32_t start, uint32_t size) {
    head = (header_t*)start;
    head->size = size - sizeof(header_t);
    head->next = NULL;
    head->is_free = 1;
    heap_total = size;
}

void* kmalloc(size_t size) {
    header_t* curr = head;
    while (curr) {
        if (curr->is_free && curr->size >= size) {
            // Can we split?
            if (curr->size > size + sizeof(header_t) + 16) {
                header_t* split = (header_t*)((char*)curr + sizeof(header_t) + size);
                split->size = curr->size - size - sizeof(header_t);
                split->is_free = 1;
                split->next = curr->next;
                
                curr->size = size;
                curr->next = split;
            }
            curr->is_free = 0;
            heap_used += curr->size + sizeof(header_t);
            return (void*)((char*)curr + sizeof(header_t));
        }
        curr = curr->next;
    }
    return NULL;
}

void kfree(void* ptr) {
    if (!ptr) return;
    header_t* header = (header_t*)((char*)ptr - sizeof(header_t));
    header->is_free = 1;
    heap_used -= (header->size + sizeof(header_t));
    
    // Coalesce adjacent free blocks
    header_t* curr = head;
    while (curr) {
        if (curr->is_free && curr->next && curr->next->is_free) {
            curr->size += curr->next->size + sizeof(header_t);
            curr->next = curr->next->next;
        } else {
            curr = curr->next;
        }
    }
}

uint32_t kheap_get_used() {
    return heap_used;
}

uint32_t kheap_get_free() {
    return heap_total - heap_used;
}
