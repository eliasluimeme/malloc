#include "malloc.h"

/*
** start_malloc: internal malloc without locking.
** 1. Align size to 16 bytes
** 2. Try reusing a freed block
** 3. Otherwise find/create a heap and append a new block
*/
void	*start_malloc(size_t size)
{
	t_heap	*heap;
	t_block	*block;

	void	*ptr;

	if (!size || size > SIZE_MAX - 4096)
		return (NULL);
	size = (size + 15) & ~15;
	block = find_free_block(size, &heap);
	if (block)
	{
		split_block(block, size, heap);
		ptr = BLOCK_SHIFT(block);
	}
	else
	{
		heap = get_heap_for_size(size);
		if (!heap)
			return (NULL);
		ptr = append_block(heap, size);
	}
	if (ptr && check_env(ENV_SCRIBBLE))
		ft_memset(ptr, 0xAA, size);
	log_event("malloc", ptr, size);
	return (ptr);
}

void	*malloc(size_t size)
{
	void	*res;

	pthread_mutex_lock(&g_malloc_mutex);
	res = start_malloc(size);
	pthread_mutex_unlock(&g_malloc_mutex);
	return (res);
}
