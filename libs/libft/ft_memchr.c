/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psleziak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/17 21:41:39 by psleziak          #+#    #+#             */
/*   Updated: 2021/05/20 23:37:02 by psleziak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned int	i;
	unsigned char	*a;

	a = (unsigned char *) s;
	i = 0;
	while (i < n)
	{
		if (a[i] == (unsigned char ) c)
			return (&a[i]);
		i++;
	}	
	return (0);
}
