/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psleziak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/17 22:57:28 by psleziak          #+#    #+#             */
/*   Updated: 2021/05/20 17:35:50 by psleziak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	unsigned int	x;
	char			*a;
	unsigned int	i;

	if (!s)
		return (NULL);
	i = -1;
	if (start >= ft_strlen(s))
	{	
		a = ft_calloc(1, sizeof(char));
		if (!a)
			return (NULL);
		return (a);
	}
	if (start + len < ft_strlen(s))
		x = len;
	else
		x = ft_strlen(s) - start;
	a = ft_calloc((x + 1), sizeof(char));
	if (!a)
		return (NULL);
	while (++i < x)
		a[i] = s[start + i];
	return (a);
}
