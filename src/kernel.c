#include "kernel.h"
#include "idt/idt.h"
#include "memory/heap/kheap.h"
#include "memory/paging/paging.h"
#include "disk/disk.h"
#include "fs/pparser.h"
#include "fs/file.h"
#include "string/string.h"
#include "disk/streamer.h"

uint16_t* video_mem = 0;
uint16_t terminalRow = 0;
uint16_t terminalCol = 0;

uint16_t terminal_make_char(char, char);
void terminalPutChar(int, int, char, char);
void terminalWriteChar(char, char);



void terminal_initialize()
{
	video_mem = (uint16_t*)(0xB8000);
	terminalRow = 0;
	terminalCol = 0;
	for(int y = 0; y < VGA_HEIGHT; y++)
	{
		for(int x = 0; x < VGA_WIDTH; x++)
		{
			terminalPutChar(x, y, ' ', 0);
		}
	}
}


uint16_t terminal_make_char(char c, char color)
{
	return (color << 8) | c;
}
void terminalPutChar(int x, int y, char c, char color)
{
	video_mem[(y * VGA_WIDTH) + x] = terminal_make_char(c, color);

}
void terminalWriteChar(char c, char color)
{
	if(c == '\n')
	{
		terminalRow += 1;
		terminalCol = 0;
		return;
	}
	terminalPutChar(terminalCol, terminalRow, c, color);
	terminalCol += 1;
	if(terminalCol >= VGA_WIDTH)
	{
		terminalCol = 0;
		terminalRow += 1;
	}
}

void print(const char* str)
{
	size_t len = strlen(str);
	for(int i = 0; i < len; i++)
	{
		terminalWriteChar(str[i], 15);
	}
}
static struct paging_4gb_chunk* kernel_chunk = 0;

void panic(const char* msg)
{
	print(msg);
	while(1) {}
}

void kernel_main()
{
	terminal_initialize();
	// initialize the heap
	kheap_init();

	// Initialize Filesystem
	fs_init();

	//Search and initialize the disk
	disk_search_and_init();

	//initialize the intrrupt descriptor table
	idt_init();
	
	// Setup Paging
	kernel_chunk = paging_new_4gb(PAGING_IS_WRITEABLE | PAGING_IS_PRESENT | PAGING_ACCESS_FROM_ALL);
	// switch to kernel Paging switch
	paging_switch(paging_4gb_chunk_get_directory(kernel_chunk));
	// Enable Paging
	enable_paging();

	// Enable system interrupts
	enable_interrupts();
	int fd = fopen("0:/hello.txt", "r");
	if (fd)
	{
		print("WE Opened hello.txt\n");
		char buf[14];
		fseek(fd, 2, SEEK_SET);
		fread(buf, 11, 1, fd);
		buf[13] = 0x00;
		print(buf);
		fclose(fd);
		print("fileclosed");
	}

	while(1){}



}	
