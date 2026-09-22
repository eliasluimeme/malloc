#include "malloc.h"

/*
** Search all heaps of matching group for a freed block large enough.
** Sets *out_heap to the containing heap on success.
*/
t_block	*find_free_block(size_t size, t_heap **out_heap)
{
	t_heap			*heap;
	t_block			*block;
	t_heap_group	group;

	group = get_heap_group(size);
	heap = g_heap_anchor;
	while (heap)
	{
		if (heap->group == group && heap->block_count > 0)
		{
			block = (t_block *)HEAP_SHIFT(heap);
			while (block)
			{
				if (block->freed && block->data_size >= size)
				{
					*out_heap = heap;
					return (block);
				}
				block = block->next;
			}
		}
		heap = heap->next;
	}
	*out_heap = NULL;
	return (NULL);
}
