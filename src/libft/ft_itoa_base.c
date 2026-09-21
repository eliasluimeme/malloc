#include "malloc.h"

static void	ft_print_number_fd(size_t nb, char base, int fd)
{
	char	c;
	char	*digits;

	digits = "0123456789abcdef";
	if (nb >= (size_t)base)
		ft_print_number_fd(nb / base, base, fd);
	c = digits[nb % base];
	write(fd, &c, 1);
}

void	ft_itoa_base_fd(size_t nb, char base, char length, int prefix, int fd)
{
	size_t	tmp;
	int		digits;

	if (prefix)
		ft_putstr_fd("0x", fd);
	digits = 0;
	tmp = nb;
	while (tmp > 0)
	{
		digits++;
		tmp /= base;
	}
	if (nb == 0)
		digits = 1;
	while (digits < length)
	{
		write(fd, "0", 1);
		digits++;
	}
	ft_print_number_fd(nb, base, fd);
}

void	ft_itoa_base(size_t nb, char base, char length, int prefix)
{
	ft_itoa_base_fd(nb, base, length, prefix, 1);
}
