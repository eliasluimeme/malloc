#include "malloc.h"

static void	merge_next(t_heap *heap, t_block *block)
{
	if (block->next && block->next->freed)
	{
		block->data_size += block->next->data_size + sizeof(t_block);
		if (block->next->next)
			block->next->next->prev = block;
		block->next = block->next->next;
		heap->block_count--;
	}
}

static t_block	*merge_prev(t_heap *heap, t_block *block)
{
	if (block->prev && block->prev->freed)
	{
		block->prev->data_size += block->data_size + sizeof(t_block);
		block->prev->next = block->next;
		if (block->next)
			block->next->prev = block->prev;
		heap->block_count--;
		return (block->prev);
	}
	return (NULL);
}

/*
** Merge a freed block with adjacent freed blocks (defragmentation).
** Returns the resulting merged block.
*/
t_block	*merge_blocks(t_heap *heap, t_block *block)
{
	t_block	*merged;

	merge_next(heap, block);
	merged = merge_prev(heap, block);
	if (merged)
		return (merged);
	return (block);
}
