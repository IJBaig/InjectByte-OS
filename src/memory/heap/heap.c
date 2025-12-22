#include "heap.h"
#include "kernel.h"
#include <stdbool.h>
#include "status.h"
#include "memory/memory.h"


static int heap_validate_table(void* ptr, void* end, struct heap_table* table)
{
	int res = 0;
	size_t table_size = (size_t)(end - ptr);
	size_t total_blocks = table_size / INJECTBYTE_HEAP_BLOCK_SIZE;
	if(table->total != total_blocks)
	{
		res = -EINVARG;
		goto out;
	}

out:
	return res;
}

static bool heap_validate_alignment(void* ptr)
{
	return ((unsigned int)ptr % INJECTBYTE_HEAP_BLOCK_SIZE) == 0;
}


int heap_create(struct heap* heap, void* ptr, void* end, struct heap_table* table)
{
	int res = 0;
	
	if(!heap_validate_alignment(ptr) || !heap_validate_alignment(end))
	{
		res = -EINVARG;
		goto out;
	}
	memset(heap, 0, sizeof(struct heap));
	heap->saddr = ptr;
	heap->table = table;

	res = heap_validate_table(ptr, end, table);
	if(res < 0)
	{
		goto out;
	}
	size_t table_size = sizeof(HEAP_BLOCK_TABLE_ENTRY) * table->total;
	memset(table->entries, HEAP_BLOCK_TABLE_ENTRY_FREE, table_size);

out:
	return res;
}

static uint32_t heapAlignValueToUpper(uint32_t val)
{
	if((val % INJECTBYTE_HEAP_BLOCK_SIZE) == 0)
	{
		return val;
	}
	val = (val - (val % INJECTBYTE_HEAP_BLOCK_SIZE));
	val += INJECTBYTE_HEAP_BLOCK_SIZE;
	return val;
}
static int heap_get_entry_type(HEAP_BLOCK_TABLE_ENTRY entry)
{
	return entry & 0x0f;
}

int heap_get_start_block(struct heap* heap, uint32_t totalBlocks)
{
	struct heap_table* table = heap->table;
	int bc = 0;
	int bs = -1;

	for (size_t i = 0; i < table->total; i++)
	{
		if(heap_get_entry_type(table->entries[i]) != HEAP_BLOCK_TABLE_ENTRY_FREE)
		{
			bc = 0;
			bs = -1;
			continue;	
		}
		if(bs == -1)
		{
			bs = i;
		}
		bc++;
		if(bc == totalBlocks)
		{
			break;
		}
	}
	if(bs == -1)
	{
		return -ENOMEM;
	}
	return bs;
}

void* heap_block_to_address(struct heap* heap, int block)
{
	return heap->saddr + (block * INJECTBYTE_HEAP_BLOCK_SIZE);
}

void heap_mark_blocks_taken(struct heap* heap, int startBlock, int totalBlocks)
{
	int endBlock = (startBlock + totalBlocks)-1;
	HEAP_BLOCK_TABLE_ENTRY entry = HEAP_BLOCK_TABLE_ENTRY_TAKEN | HEAP_BLOCK_IS_FIRST;
	if(totalBlocks > 1)
	{
		entry |= HEAP_BLOCK_HAS_NEXT;
	}

	for(int i = startBlock; i <= endBlock; i++)
	{
		heap->table->entries[i] = entry;
		entry = HEAP_BLOCK_TABLE_ENTRY_TAKEN;
		if(i != endBlock -1)
		{
			entry |= HEAP_BLOCK_HAS_NEXT;
		}
	}
}

void* heap_malloc_blocks(struct heap* heap, uint32_t totalBlocks)
{
	void* address = 0;
	int start_block = heap_get_start_block(heap, totalBlocks);
	if(start_block < 0)
	{
		goto out;
	}
	address = heap_block_to_address(heap, start_block);
	//Mark the block as Taken
	heap_mark_blocks_taken(heap, start_block, totalBlocks);

out:
	return address;
}

int heap_address_to_block(struct heap* heap, void* addr)
{
	return ((int)(addr - heap->saddr)) / INJECTBYTE_HEAP_BLOCK_SIZE;
}

void heap_mark_blocks_free(struct heap* heap, int starting_block)
{
	struct heap_table* table = heap->table;
	for(int i = starting_block; i < (int)table->total; i++)
	{
		HEAP_BLOCK_TABLE_ENTRY entry = table->entries[i];
		table->entries[i] = HEAP_BLOCK_TABLE_ENTRY_FREE;
		if(!(entry & HEAP_BLOCK_HAS_NEXT))
		{
			break;
		}
	}
			

}

void* heap_malloc(struct heap* heap, size_t size)
{
	size_t alignedSize = heapAlignValueToUpper(size);
	uint32_t total_blocks = alignedSize / INJECTBYTE_HEAP_BLOCK_SIZE;
	return heap_malloc_blocks(heap, total_blocks);
}

void heap_free(struct heap* heap, void* ptr)
{
	heap_mark_blocks_free(heap, heap_address_to_block(heap, ptr));
}

