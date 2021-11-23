/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memccpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psleziak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/17 21:34:58 by psleziak          #+#    #+#             */
/*   Updated: 2021/05/20 22:56:32 by psleziak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memccpy(void *dest, const void *src, int c, size_t n)
{
	size_t			i;
	unsigned char	*x;
	unsigned char	*y;

	x = (unsigned char *)dest;
	y = (unsigned char *)src;
	i = 0;
	while (i < n)
	{
		x[i] = y[i];
		if (y[i] == (unsigned char )c)
		{
			i++;
			return ((char *)&x[i]);
		}
		i++;
	}
	return (NULL);
}
