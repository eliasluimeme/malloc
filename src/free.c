#include "malloc.h"

/*
** start_free: internal free without locking.
** 1. Validate pointer via linear scan
** 2. Mark as freed
** 3. Defragment (merge adjacent freed blocks)
** 4. Reclaim space if last block
** 5. Munmap heap if empty
*/
void	start_free(void *ptr)
{
	t_heap	*heap;
	t_block	*block;

	heap = g_heap_anchor;
	if (!ptr || !heap)
		return ;
	search_ptr(&heap, &block, heap, ptr);
	if (!block || !heap || block->freed)
		return ;
	log_event("free", ptr, block->data_size);
	if (check_env(ENV_SCRIBBLE))
		ft_memset(ptr, 0x55, block->data_size);
	block->freed = 1;
	block = merge_blocks(heap, block);
	remove_last_block(heap, block);
	delete_heap_if_empty(heap);
}

void	free(void *ptr)
{
	pthread_mutex_lock(&g_malloc_mutex);
	start_free(ptr);
	pthread_mutex_unlock(&g_malloc_mutex);
}
