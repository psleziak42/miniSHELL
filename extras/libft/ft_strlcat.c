/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psleziak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/17 22:14:03 by psleziak          #+#    #+#             */
/*   Updated: 2021/05/17 22:17:56 by psleziak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t sizedst)
{
	unsigned int	i;
	unsigned int	j;
	unsigned int	c;

	i = 0;
	j = 0;
	while (src[j] != '\0')
		j++;
	while (dst[i] != '\0')
		i++;
	if (sizedst > i)
		c = i + j;
	else
		c = j + sizedst;
	j = 0;
	while (src[j] != '\0' && i + 1 < sizedst)
	{
		dst[i] = src[j];
		j++;
		i++;
	}
	dst[i] = '\0';
	return (c);
}
