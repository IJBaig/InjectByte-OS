#ifndef KHEAP_H
#define KHEAP_H

#include <stdint.h>
#include <stddef.h>

void kheap_init();
void* kmalloc(size_t);
void kfree(void* ptr);
void* kzalloc(size_t);

#endif
