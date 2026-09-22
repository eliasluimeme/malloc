#include "malloc.h"

t_block	*get_last_block(t_block *block)
{
	while (block->next)
		block = block->next;
	return (block);
}

void	*append_block(t_heap *heap, size_t size)
{
	t_block	*new_block;
	t_block	*last;

	new_block = (t_block *)HEAP_SHIFT(heap);
	last = NULL;
	if (heap->block_count)
	{
		last = get_last_block(new_block);
		new_block = (t_block *)(BLOCK_SHIFT(last) + last->data_size);
	}
	init_block(new_block, size);
	if (last)
	{
		last->next = new_block;
		new_block->prev = last;
	}
	heap->block_count++;
	heap->free_size -= (size + sizeof(t_block));
	return (BLOCK_SHIFT(new_block));
}
