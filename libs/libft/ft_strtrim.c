/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psleziak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/17 22:35:29 by psleziak          #+#    #+#             */
/*   Updated: 2021/05/20 23:24:36 by psleziak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_check_1(char const *s1, char const *set)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (set[j] != '\0')
	{
		if (s1[i] == set[j])
		{
			i++;
			j = 0;
		}
		else
			j++;
	}
	return (i);
}

int	ft_check_2(char const *s1, char const *set, int i)
{
	int	j;
	int	k;

	j = 0;
	k = 0;
	while (set[j] != '\0')
	{
		if (s1[i] == set[j])
		{
			i--;
			j = 0;
			k++;
		}
		else
			j++;
	}
	return (k);
}

int	ft_k(char const *s1, char const *set, unsigned int j, unsigned int i)
{
	unsigned int	k;

	if (j != ft_strlen(s1))
		k = ft_check_2(s1, set, j + i - 1);
	else
		k = 0;
	return (k);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	unsigned int	i;
	unsigned int	j;
	unsigned int	l;
	unsigned int	k;
	char			*trim;

	if (!s1 || !set)
		return (0);
	i = 0;
	l = 0;
	j = ft_check_1(s1, set);
	while (s1[j + i] != '\0')
		i++;
	k = ft_k(s1, set, j, i);
	trim = ft_calloc((i - k + 1), sizeof(char));
	if (!trim)
		return (0);
	while (i-- - k > 0)
	{
		trim[l] = s1[j + l];
		l++;
	}
	return (trim);
}		
