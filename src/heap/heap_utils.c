#include "malloc.h"

t_heap_group	get_heap_group(size_t size)
{
	if (size <= (size_t)TINY_BLOCK_SIZE)
		return (TINY);
	else if (size <= (size_t)SMALL_BLOCK_SIZE)
		return (SMALL);
	return (LARGE);
}

/*
** For LARGE allocations, round up to the nearest page boundary.
*/
size_t	get_heap_alloc_size(size_t block_size)
{
	t_heap_group	group;
	size_t			raw;
	size_t			page;

	group = get_heap_group(block_size);
	if (group == TINY)
		return ((size_t)TINY_HEAP_ALLOCATION_SIZE);
	else if (group == SMALL)
		return ((size_t)SMALL_HEAP_ALLOCATION_SIZE);
	raw = block_size + sizeof(t_heap) + sizeof(t_block);
	page = (size_t)getpagesize();
	return (((raw + page - 1) / page) * page);
}

t_heap	*get_last_heap(t_heap *heap)
{
	if (!heap)
		return (NULL);
	while (heap->next)
		heap = heap->next;
	return (heap);
}
