#include "malloc.h"

void	init_block(t_block *block, size_t size)
{
	block->prev = NULL;
	block->next = NULL;
	block->data_size = size;
	block->freed = 0;
}
