#include "malloc.h"

static void	print_heap_header(const char *name, t_heap *heap)
{
	ft_putstr(name);
	ft_putstr(" : ");
	ft_itoa_base((size_t)heap, 16, 0, 1);
	ft_putstr("\n");
}

static size_t	print_block_list(t_block *block)
{
	char	*start;
	char	*end;
	size_t	total;

	total = 0;
	while (block)
	{
		if (!block->freed)
		{
			start = (char *)BLOCK_SHIFT(block);
			end = start + block->data_size;
			ft_itoa_base((size_t)start, 16, 0, 1);
			ft_putstr(" - ");
			ft_itoa_base((size_t)end, 16, 0, 1);
			ft_putstr(" : ");
			ft_itoa_base(block->data_size, 10, 0, 0);
			ft_putstr(" bytes\n");
			total += block->data_size;
		}
		block = block->next;
	}
	return (total);
}

static void	start_show_alloc_mem(void)
{
	t_heap	*heap;
	size_t	total;

	total = 0;
	heap = get_last_heap(g_heap_anchor);
	while (heap)
	{
		if (heap->group == TINY)
			print_heap_header("TINY", heap);
		else if (heap->group == SMALL)
			print_heap_header("SMALL", heap);
		else
			print_heap_header("LARGE", heap);
		if (heap->block_count)
			total += print_block_list((t_block *)HEAP_SHIFT(heap));
		heap = heap->prev;
	}
	ft_putstr("Total : ");
	ft_itoa_base(total, 10, 0, 0);
	ft_putstr(" bytes\n");
}

void	show_alloc_mem(void)
{
	pthread_mutex_lock(&g_malloc_mutex);
	start_show_alloc_mem();
	pthread_mutex_unlock(&g_malloc_mutex);
}
