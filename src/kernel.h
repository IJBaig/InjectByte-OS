#ifndef KERNEL_H
#define KERNEL_H

#include <stdint.h>
#include <stddef.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 20
#define INJECTBYTE_MAX_PATH 108

void kernel_main();
void print(const char*);
#endif
