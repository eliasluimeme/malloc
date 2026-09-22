#include "malloc.h"

/*
** Find an existing heap with enough room, or create a new one.
** New heaps are inserted at the head of the global list.
*/
static t_heap	*find_available_heap(t_heap_group group, size_t required_size)
{
	t_heap	*el;

	el = g_heap_anchor;
	while (el)
	{
		if (el->group == group && el->free_size >= required_size)
			return (el);
		el = el->next;
	}
	return (NULL);
}

t_heap	*get_heap_for_size(size_t size)
{
	t_heap_group	group;
	t_heap			*heap;

	group = get_heap_group(size);
	if (group != LARGE)
	{
		heap = find_available_heap(group, size + sizeof(t_block));
		if (heap)
			return (heap);
	}
	heap = create_heap(group, size);
	if (!heap)
		return (NULL);
	heap->next = g_heap_anchor;
	if (g_heap_anchor)
		g_heap_anchor->prev = heap;
	g_heap_anchor = heap;
	return (heap);
}
