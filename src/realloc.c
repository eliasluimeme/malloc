#include "malloc.h"

/*
** start_realloc: internal realloc without locking.
** Handles: NULL ptr, zero size, same size, in-place shrink, fallback copy.
*/
void	*start_realloc(void *ptr, size_t size)
{
	t_heap	*heap;
	t_block	*block;
	void	*new_ptr;
	size_t	copy_size;

	if (!ptr)
		return (start_malloc(size));
	if (size == 0)
	{
		start_free(ptr);
		return (NULL);
	}
	if (size > SIZE_MAX - 4096)
		return (NULL);
	size = (size + 15) & ~15;
	heap = g_heap_anchor;
	search_ptr(&heap, &block, heap, ptr);
	if (!heap || !block || block->freed)
		return (NULL);
	if (size == block->data_size)
		return (ptr);
	if (size < block->data_size)
	{
		split_block(block, size, heap);
		return (ptr);
	}
	new_ptr = start_malloc(size);
	if (!new_ptr)
		return (NULL);
	copy_size = block->data_size;
	if (copy_size > size)
		copy_size = size;
	ft_memmove(new_ptr, ptr, copy_size);
	start_free(ptr);
	return (new_ptr);
}

void	*realloc(void *ptr, size_t size)
{
	void	*res;

	pthread_mutex_lock(&g_malloc_mutex);
	res = start_realloc(ptr, size);
	pthread_mutex_unlock(&g_malloc_mutex);
	return (res);
}
