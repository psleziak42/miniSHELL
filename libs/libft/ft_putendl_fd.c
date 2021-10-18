#include "libft.h"

void	ft_putendl_fd(char *s, int fd)
{
	int	i;

	if (s)
	{
		i = 0;
		while (s[i] != '\0')
			write(fd, &s[i++], 1);
		write(fd, "\n", 1);
	}
}
