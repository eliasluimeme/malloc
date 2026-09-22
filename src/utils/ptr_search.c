#include "malloc.h"

/*
** Linear scan all heaps and blocks to find which block owns ptr.
** Sets *out_heap and *out_block on success, both NULL on failure.
*/
void	search_ptr(t_heap **out_heap, t_block **out_block,
		t_heap *heap, void *ptr)
{
	t_block	*block;

	while (heap)
	{
		if (heap->block_count > 0)
		{
			block = (t_block *)HEAP_SHIFT(heap);
			while (block)
			{
				if (BLOCK_SHIFT(block) == ptr)
				{
					*out_heap = heap;
					*out_block = block;
					return ;
				}
				block = block->next;
			}
		}
		heap = heap->next;
	}
	*out_heap = NULL;
	*out_block = NULL;
}
