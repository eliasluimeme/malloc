#include "malloc.h"

/*
** Check if this heap is the last remaining one of its group (TINY/SMALL).
** We keep at least one TINY and one SMALL heap alive to avoid mmap thrashing.
*/
static int	is_last_of_group(t_heap *heap)
{
	t_heap			*el;
	t_heap_group	group;
	int				count;

	group = heap->group;
	if (group == LARGE)
		return (0);
	count = 0;
	el = g_heap_anchor;
	while (el)
	{
		if (el->group == group)
			count++;
		el = el->next;
	}
	return (count == 1);
}

void	delete_heap_if_empty(t_heap *heap)
{
	if (heap->block_count)
		return ;
	if (is_last_of_group(heap))
		return ;
	if (heap->prev)
		heap->prev->next = heap->next;
	if (heap->next)
		heap->next->prev = heap->prev;
	if (heap == g_heap_anchor)
		g_heap_anchor = heap->next;
	munmap(heap, heap->total_size);
}
