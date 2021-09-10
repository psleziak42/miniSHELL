/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psleziak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/17 21:51:04 by psleziak          #+#    #+#             */
/*   Updated: 2021/05/20 22:57:47 by psleziak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	int				i;
	unsigned char	*a;
	unsigned char	*b;

	if (!dest && !src)
		return (dest);
	a = (unsigned char *)src;
	b = (unsigned char *)dest;
	i = (int)n - 1;
	if (b > a)
	{
		while (i >= 0)
		{
			b[i] = a[i];
			i--;
		}
	}
	else
		ft_memcpy(b, a, n);
	return (dest);
}
