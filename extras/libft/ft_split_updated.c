/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psleziak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/19 17:55:03 by psleziak          #+#    #+#             */
/*   Updated: 2021/09/22 16:41:17 by psleziak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_copy(char *a, char const *s, int i, int k)
{
	int	z;

	z = -1;
	while (++z < k)
		a[z] = s[i - k + z];
	printf("%s\n", a);
	return (a);
}	

static int	ft_word(char const *s, int *i, char c)
{
	int	k;

	k = 0;
	while (s[*i] == c)
		(*i)++;
	while (s[*i] != c && s[*i] != '\0')
	{
		if (s[*i] == '\'' || s[*i] == '\"') 
		{
			g_quote.on_quote = 1;
			g_quote.quote = s[*i];
			(*i)++;
			return (k);
		}
		k++;
		(*i)++;
	}
	if (k == 0)
		return (0);
	else
		return (k);
}

static int	ft_count(char const *s, char c) // tutaj by pasowalo zeby ucinac i kopiowac do " lub ' i jesli znajde to ustawiac zmienna na 1 i przenosic do funkcji ktora stworzy string '
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (s[i] != '\0')
	{
		if ((s[0] != c && i == 0))
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

static char	*ft_quote_arg(char const *s, char c, int *i)
{
	int		j;
	char	*handmade_arg;

	j = *i;
	while (s[j] != c)
	{
		if (s[j] == '\0')
		{
			printf("quote isnt closed\n");
			//ft_error_exit("quotes arent closed");
			return 0;
		}
		j++;
	}
	handmade_arg = malloc((j + 1) * sizeof(char));
	if (!handmade_arg)
		return (0);
	j = -1;
	while (s[*i] != c)
	{
		printf("%c\n", s[*i]);	
		handmade_arg[++j] = s[(*i)++];	
	}
	handmade_arg[++j] = '\0';
	g_quote.on_quote = 0;
	return (handmade_arg);
}

char	**ft_split_updated(char const *s, char c)
{
	int		i;
	int		k;
	int		y;
	char	*small;
	char	**big;

	ft_memset(&g_quote, 0, sizeof(g_quote));
	if (!s)
		return (0);
	i = 0;
	y = -1;
	big = ft_calloc((ft_count(s, c) + 1), sizeof(char *));
	if (!big)
		return (0);
	while (s[i] != '\0')
	{
		printf("%d\n", g_quote.on_quote);
		if (g_quote.on_quote)
			big[++y] = ft_quote_arg(s, g_quote.quote, &i);
		else
		{
			k = ft_word(s, &i, c);
			if (!k && !g_quote.on_quote)
				break ; // bys moze continue jest potrzebne zeby nie wychodzil jak znajdzie " na poczatku.
			small = ft_calloc((k + 1), sizeof(char));
			if (!small)
				return (0);
			big[++y] = ft_copy(small, s, i, k);
		}
	}
	return (big);
}
