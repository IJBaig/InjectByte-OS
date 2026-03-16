#ifndef KERNEL_H
#define KERNEL_H

#include <stdint.h>
#include <stddef.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 20
#define INJECTBYTE_MAX_PATH 108

void terminalWriteChar(char c, char colour);
void panic(const char*);
void kernel_main();
void print(const char*);
void kernel_page();
void kernel_registers();


#define ERROR(value) (void*)(value)
#define ERROR_I(value) (int)(value)
#define ISERR(value) ((int)value < 0)

#endif
