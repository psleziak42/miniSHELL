/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psleziak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/18 00:17:26 by psleziak          #+#    #+#             */
/*   Updated: 2021/05/19 22:09:29 by psleziak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_count_i(int *n)
{
	int	i;
	int	z;

	if (*n < 0)
	{
		*n = -(*n);
		z = *n;
		i = 1;
	}
	else
	{
		z = *n;
		i = 0;
	}
	while (z > 0)
	{
		z = z /10;
		i++;
	}
	return (i);
}

char	*ft_itoa(int n)
{
	int		i;
	char	x;
	char	*itoa_buffer;

	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	if (n == 0)
		return (ft_strdup("0"));
	i = ft_count_i(&n);
	itoa_buffer = ft_calloc((i + 1), sizeof(char));
	if (!itoa_buffer)
		return (0);
	while (i--)
	{	
		x = (n %10 + 48);
		itoa_buffer[i] = x;
		n = n /10;
		if (n == 0 && i == 1)
		{
			i--;
			itoa_buffer[i] = '-';
			break ;
		}
	}
	return (itoa_buffer);
}
