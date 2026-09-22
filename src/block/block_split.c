#include "malloc.h"

/*
** Split a freed block into [used (size)] + [free (remainder)].
** Only creates the free remainder if it's large enough to hold
** a block header + at least 16 bytes of data.
*/
void	split_block(t_block *block, size_t size, t_heap *heap)
{
	t_block	*free_block;
	size_t	remainder;

	remainder = block->data_size - size;
	if (remainder >= sizeof(t_block) + 16)
	{
		free_block = (t_block *)(BLOCK_SHIFT(block) + size);
		init_block(free_block, remainder - sizeof(t_block));
		free_block->freed = 1;
		free_block->prev = block;
		free_block->next = block->next;
		if (block->next)
			block->next->prev = free_block;
		block->next = free_block;
		block->data_size = size;
		heap->block_count++;
		free_block = merge_blocks(heap, free_block);
		remove_last_block(heap, free_block);
	}
	block->freed = 0;
}
