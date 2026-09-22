#include "malloc.h"

static void	print_heap_header(const char *name, t_heap *heap)
{
	ft_putstr(name);
	ft_putstr(" : ");
	ft_itoa_base((size_t)heap, 16, 0, 1);
	ft_putstr(" (total: ");
	ft_itoa_base(heap->total_size, 10, 0, 0);
	ft_putstr(" bytes, free: ");
	ft_itoa_base(heap->free_size, 10, 0, 0);
	ft_putstr(" bytes, blocks: ");
	ft_itoa_base(heap->block_count, 10, 0, 0);
	ft_putstr(")\n");
}

static void	print_hex_dump_line(const unsigned char *data,
		size_t offset, size_t line_len)
{
	size_t	i;
	char	c;

	ft_itoa_base((size_t)(data + offset), 16, 0, 1);
	ft_putstr("  ");
	i = 0;
	while (i < 16)
	{
		if (i == 8)
			ft_putstr(" ");
		if (i < line_len)
			ft_itoa_base(data[offset + i], 16, 2, 0);
		else
			ft_putstr("  ");
		ft_putstr(" ");
		i++;
	}
	ft_putstr(" |");
	i = 0;
	while (i < line_len)
	{
		c = (char)data[offset + i];
		if (c >= 32 && c <= 126)
			write(1, &c, 1);
		else
			write(1, ".", 1);
		i++;
	}
	ft_putstr("|\n");
}

static void	dump_block_data(t_block *block)
{
	const unsigned char	*data;
	size_t				offset;
	size_t				chunk;
	size_t				max_dump;

	data = (const unsigned char *)BLOCK_SHIFT(block);
	offset = 0;
	max_dump = block->data_size;
	if (max_dump > 256)
		max_dump = 256;
	while (offset < max_dump)
	{
		chunk = max_dump - offset;
		if (chunk > 16)
			chunk = 16;
		print_hex_dump_line(data, offset, chunk);
		offset += chunk;
	}
	if (block->data_size > 256)
	{
		ft_putstr("  ... (");
		ft_itoa_base(block->data_size - 256, 10, 0, 0);
		ft_putstr(" bytes omitted)\n");
	}
}

static size_t	dump_block_list(t_block *block)
{
	char	*start;
	char	*end;
	size_t	total;

	total = 0;
	while (block)
	{
		start = (char *)BLOCK_SHIFT(block);
		end = start + block->data_size;
		if (!block->freed)
		{
			ft_itoa_base((size_t)start, 16, 0, 1);
			ft_putstr(" - ");
			ft_itoa_base((size_t)end, 16, 0, 1);
			ft_putstr(" : ");
			ft_itoa_base(block->data_size, 10, 0, 0);
			ft_putstr(" bytes\n");
			dump_block_data(block);
			total += block->data_size;
		}
		else
		{
			ft_itoa_base((size_t)start, 16, 0, 1);
			ft_putstr(" - ");
			ft_itoa_base((size_t)end, 16, 0, 1);
			ft_putstr(" : ");
			ft_itoa_base(block->data_size, 10, 0, 0);
			ft_putstr(" bytes (FREED)\n");
		}
		block = block->next;
	}
	return (total);
}

static void	start_show_alloc_mem_ex(void)
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
			total += dump_block_list((t_block *)HEAP_SHIFT(heap));
		heap = heap->prev;
	}
	ft_putstr("Total : ");
	ft_itoa_base(total, 10, 0, 0);
	ft_putstr(" bytes\n");
}

void	show_alloc_mem_ex(void)
{
	pthread_mutex_lock(&g_malloc_mutex);
	start_show_alloc_mem_ex();
	pthread_mutex_unlock(&g_malloc_mutex);
}

void	show_alloc_mem_hex(void)
{
	show_alloc_mem_ex();
}
