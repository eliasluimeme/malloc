#include "malloc.h"

void	*start_calloc(size_t count, size_t size)
{
	void	*ptr;
	size_t	total;

	if (!count || !size)
		return (NULL);
	if (count > SIZE_MAX / size)
		return (NULL);
	total = count * size;
	ptr = start_malloc(total);
	if (!ptr)
		return (NULL);
	ft_bzero(ptr, total);
	return (ptr);
}

void	*calloc(size_t count, size_t size)
{
	void	*res;

	pthread_mutex_lock(&g_malloc_mutex);
	res = start_calloc(count, size);
	pthread_mutex_unlock(&g_malloc_mutex);
	return (res);
}
