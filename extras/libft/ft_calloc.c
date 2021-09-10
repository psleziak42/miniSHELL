/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psleziak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/17 21:31:47 by psleziak          #+#    #+#             */
/*   Updated: 2021/05/17 21:31:49 by psleziak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	unsigned int	i;
	char			*a;

	a = malloc(nmemb * size);
	if (a == 0)
		return (0);
	i = 0;
	while (i < nmemb * size)
	{
		a[i] = '\0';
		i++;
	}
	return (a);
}
