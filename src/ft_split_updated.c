/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_updated.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosilva <tosilva@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/19 17:55:03 by psleziak          #+#    #+#             */
/*   Updated: 2021/10/18 18:56:12 by tosilva          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*ft_copy(char *a, char const *s, int i, int k)
{
	int	z;

	z = -1;
	while (++z < k)
		a[z] = s[i - k + z];
	return (a);
}	

static int	ft_word(char const *s, int *i, char c)
{
	int	k;
	int	temp_i;

	temp_i = *i;
	k = 0;
	while (s[*i] == c)
		(*i)++;
	while (s[*i] != c && s[*i] != '\0')
	{
		if (s[*i] == '\'' || s[*i] == '\"') 
		{
			g_mini.quote.on_quote = 1;
			g_mini.quote.quote = s[*i];
			*i = temp_i;
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

static char *get_content_from_keyword(char *str, int length)
{
	t_list *temp;

	temp = g_mini.env;
	while (temp->next)
	{
		if (ft_strncmp(str, temp->keyword, length) == 0)
			return (temp->content);
		temp = temp->next;
	}
	return (NULL);
}

static int	is_end_character(char c)
{
	return (c == ' '
			|| c == '\''
			|| c == '\"');
}

static int	ft_keyword_length(char *str)
{
	int		length;

	length = 0;
	while (is_end_character(str[length]))
		length++;
	return (length);
}

static int	ft_get_countent_length(char *str, int *j)
{
	int		length;
	char	*shell_variable;

	length = ft_keyword_length(str);
	shell_variable = get_content_from_keyword(str, length);
	if (!shell_variable)
		return (0);
	*j += length;
	return (ft_strlen(shell_variable));
}

static char	*ft_quote_arg(char const *s, char c, int *i)
{
	int		j;
	int		length;
	char	*handmade_arg;
	char	*env_var;
	int		env_var_length;

	j = *i;
	while (s[j] != c)
	{
		if (s[j] == '\0')
		{
			errno = 1;
			ft_error_handler("quotes must be closed");
			//ft_error_exit("quotes arent closed");
			return (0);
		}
		if (s[j] == '$'
			&& (g_mini.quote.quote == '\"' || !(g_mini.quote.on_quote)))
		{
			length = ft_get_countent_length((char *)&s[j + 1], &j);
		}
		else
			length++;
		j++;
	}
	handmade_arg = malloc((length + 1) * sizeof(char));
	if (!handmade_arg)
		return (0);
	j = 0;
	while (s[*i] != c)
	{
		if (s[*i] == '$'
			&& (g_mini.quote.quote == '\"' || !(g_mini.quote.on_quote)))
		{
			env_var_length = ft_keyword_length((char *)&s[*i + 1]);
			env_var = get_content_from_keyword((char *)&s[*i + 1], env_var_length);
			ft_memcpy(&handmade_arg[j], env_var, ft_strlen(env_var));
			j += env_var_length - 1;
		}
		else
			handmade_arg[j] = s[*i];
		j++;
	}
	handmade_arg[++j] = '\0';
	*i += j;
	g_mini.quote.on_quote = 0;
	return (handmade_arg);
}

char	**ft_split_updated(char const *s, char c)
{
	int		i;
	int		k;
	int		y;
	char	*small;
	char	**big;

	if (!s)
		return (0);
	ft_memset(&g_mini.quote, 0, sizeof(g_mini.quote));
	i = 0;
	y = -1;
	big = ft_calloc((ft_count(s, c) + 2), sizeof(char *));
	if (!big)
		return (0);
	while (s[i] != '\0')
	{
		if (g_mini.quote.on_quote)
			big[++y] = ft_quote_arg(s, g_mini.quote.quote, &i);
		else
		{
			k = ft_word(s, &i, c);
			if (!k && g_mini.quote.on_quote)
				continue ; // maybe continue is necesary so it wont leave this loop when " is first
			small = ft_calloc((k + 1), sizeof(char));
			if (!small)
				return (0);
			big[++y] = ft_copy(small, s, i, k);
		}
	}
	big[++y] = NULL;
	return (big);
}
