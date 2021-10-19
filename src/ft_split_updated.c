/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_updated.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosilva <tosilva@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/19 17:55:03 by psleziak          #+#    #+#             */
/*   Updated: 2021/10/19 18:48:39 by tosilva          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_count_words(char *input, char delimiter)
{
	int	nr_words;
	int	i;

	nr_words = 0;
	i = -1;
	while (input[++i])
	{
		if (!nr_words
		|| (input[i] == delimiter && input[i + 1] != delimiter))
			nr_words++;
		else if (input[i] == '\'' || input[i] == '\"')
		{
			g_mini.quote.on_quote = 1;
			g_mini.quote.quote = input[i];
			while (input[++i])
			{
				if (input[i] == g_mini.quote.quote)
				{
					g_mini.quote.on_quote = 0;
					break ;
				}
			}
			if (g_mini.quote.on_quote)
			{
				errno = 1;
				ft_error_handler("quotes must be closed");
				return (-1);
			}
		}
	}
	return (nr_words);
}

static int	ft_count_words_and_malloc(char ***argv, char *input, char delimiter)
{
	int		nr_words;

	nr_words = ft_count_words(input, delimiter);
	if (nr_words == -1)
		return (-1);
	*argv = ft_calloc((nr_words + 1), sizeof(char *));
	if (!(*argv))
		return (-1);
	return (nr_words);
}

static int	is_end_character(char c)
{
	return (c == ' '
			|| c == '\''
			|| c == '\"');
}

static int	ft_keyword_length(char *input)
{
	int		length;

	length = 0;
	while (is_end_character(input[length]))
		length++;
	return (length);
}

static char *get_content_from_keyword(char *input, int length)
{
	t_list *temp;

	temp = g_mini.env;
	while (temp->next)
	{
		if (ft_strncmp(input, temp->keyword, length) == 0)
			return (temp->content);
		temp = temp->next;
	}
	return (NULL);
}

static int	ft_get_countent_length(char *input, int kw_length)
{
	char	*shell_variable;

	shell_variable = get_content_from_keyword(input, kw_length);
	if (!shell_variable)
		return (0);
	return (ft_strlen(shell_variable));
}

static char	*ft_expand_dollar(char *input, int *i)
{
	int	arg_len;
	int	keyword_len;

	arg_len = 0;
	while (input[++(*i)] != g_mini.quote.quote)
	{
		if (input[*i] == '$')
		{
			keyword_len = ft_keyword_length(&input[*i + 1]);
			arg_len += ft_get_countent_length(&input[*i + 1], keyword_len);
			*i += keyword_len;
		}
		else
			arg_len++;
	}
}
static void	ft_copy_words(char **argv, char *input, char delimiter, int nr_words)
{
	int		word_len;
	int		wd;
	int		i;
	bool 	dollar;

	wd = -1;
	i = 0;
	dollar = FALSE;
	while (++wd < nr_words)
	{
		word_len = 0;
		ft_bzero(&g_mini.quote, sizeof(t_quote));
		while (input[i] == delimiter)
			i++;
		if (input[i] == '\'' || input[i] == '\"')
		{
			g_mini.quote.quote = input[i];
			while (input[++i] != g_mini.quote.quote)
			{
				if (input[i] == '$')
				{
					dollar = TRUE;
					argv[wd++] = ft_expand_dollar(&input[i - word_len], &i);
					break ;
				}
			}
			if (dollar)
			{
				dollar = FALSE;
				continue ;
			}
				word_len++;
			argv[wd] = ft_calloc(word_len + 1, sizeof(char));
			ft_memcpy(argv[wd], &input[i - word_len], word_len);
			i++;
		}
		else
		{
			while (input[i] && input[i] != delimiter)
			{
				word_len++;
				i++;
			}
			argv[wd] = ft_calloc(word_len + 1, sizeof(char));
			ft_memcpy(argv[wd], &input[i - (word_len)], word_len);
		}
	}
}

char	**ft_split_updated(char *input, char delimiter)
{
	char	**argv;
	int		nr_words;

	if (!input)
		return (NULL);
	ft_bzero(&(g_mini.quote), sizeof(t_quote));
	nr_words = ft_count_words_and_malloc(&argv, input, delimiter);
	if (nr_words == -1)
		return (NULL);
	ft_copy_words(argv, input, delimiter, nr_words);
	return (argv);
}
