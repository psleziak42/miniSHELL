/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psleziak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/19 17:55:03 by psleziak          #+#    #+#             */
/*   Updated: 2021/05/20 18:09:48 by psleziak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_copy(char *a, char const *s, int i, int k)
{
	int	z;

	z = -1;
	while (++z < k)
		a[z] = s[i - k + z];
	return (a);
}	

int	ft_word(char const *s, int *i, char c)
{
	int	k;

	k = 0;
	while (s[*i] == c)
		(*i)++;
	while (s[*i] != c && s[*i] != '\0')
	{
		k++;
		(*i)++;
	}
	if (k == 0)
		return (0);
	else
		return (k);
}

int	ft_count(char const *s, char c)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (s[i] != '\0')
	{
		if (s[0] != c && i == 0)
		{
			j++;
			i++;
		}
		if (s[i] == c && s[i + 1] != c && s[i + 1] != '\0')
		{
			j++;
			i++;
		}
		else
			i++;
	}
	return (j);
}

char	**ft_split(char const *s, char c)
{
	int		i;
	int		k;
	int		y;
	char	*small;
	char	**big;

	if (!s)
		return (0);
	i = 0;
	y = -1;
	big = ft_calloc((ft_count(s, c) + 1), sizeof(char *));
	if (!big)
		return (0);
	while (s[i] != '\0')
	{
		k = ft_word(s, &i, c);
		if (!k)
			break ;
		small = ft_calloc((k + 1), sizeof(char));
		if (!small)
			return (0);
		big[++y] = ft_copy(small, s, i, k);
	}
	return (big);
}
