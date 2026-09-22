#include "malloc.h"

static rlim_t	get_system_limit(void)
{
	struct rlimit	rpl;

	if (getrlimit(RLIMIT_DATA, &rpl) < 0)
		return ((rlim_t)-1);
	return (rpl.rlim_max);
}

t_heap	*create_heap(t_heap_group group, size_t block_size)
{
	size_t	heap_size;
	t_heap	*heap;

	heap_size = get_heap_alloc_size(block_size);
	if (heap_size > get_system_limit())
		return (NULL);
	heap = (t_heap *)mmap(NULL, heap_size,
			PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
	if (heap == MAP_FAILED)
		return (NULL);
	ft_bzero(heap, sizeof(t_heap));
	heap->group = group;
	heap->total_size = heap_size;
	heap->free_size = heap_size - sizeof(t_heap);
	heap->block_count = 0;
	return (heap);
}
