#include "malloc.h"

/*
** If the block is freed and is the last in the heap,
** remove it and reclaim its space for the heap's free pool.
*/
void	remove_last_block(t_heap *heap, t_block *block)
{
	t_block	*prev;

	while (block && block->freed && !block->next)
	{
		prev = block->prev;
		if (prev)
			prev->next = NULL;
		heap->free_size += block->data_size + sizeof(t_block);
		heap->block_count--;
		ft_bzero(block, sizeof(t_block));
		block = prev;
	}
}
