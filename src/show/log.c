#include "malloc.h"

void	log_event(const char *action, void *ptr, size_t size)
{
	if (!check_env(ENV_LOGGING))
		return ;
	ft_putstr_fd("[", 2);
	ft_putstr_fd(action, 2);
	ft_putstr_fd("] ", 2);
	ft_itoa_base_fd((size_t)ptr, 16, 0, 1, 2);
	ft_putstr_fd(" (", 2);
	ft_itoa_base_fd(size, 10, 0, 0, 2);
	ft_putstr_fd(" bytes)\n", 2);
}
