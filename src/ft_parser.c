/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parser.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosilva <tosilva@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/23 16:21:54 by tosilva           #+#    #+#             */
/*   Updated: 2021/10/25 18:25:14 by tosilva          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_pipe(char *str)
{
	return (str[0] == '|'
		|| str[0] == '<'
		|| (str[0] == '<' && str[1] == '<')
		|| str[0] == '>'
		|| (str[0] == '>' && str[1] == '>'));
}

static int	is_closed_quote(int input_i)
{
	int i;

	i = input_i;
	while (g_mini.input[++i])
	{
		if (g_mini.input[i] == g_mini.quote.quote)
		{
			g_mini.quote.on_quote = 0;
			break ;
		}
	}
	if (g_mini.quote.on_quote)
	{
		errno = 1;
		ft_error_handler("Quotes must be closed");
		return (-1);
	}
	return (i);
}

static int	count_words_until_pipe(int input_i)
{
	int	nr_words;
	int	i;

	nr_words = 0;
	i = input_i - 1;
	while (g_mini.input[++i] && !is_pipe(&g_mini.input[i]))
	{
		if (!nr_words || (g_mini.input[i] == ' '
				&& g_mini.input[i + 1] != ' ' && !is_pipe(&g_mini.input[i + 1])))
			nr_words++;
		else if (g_mini.input[i] == '\'' || g_mini.input[i] == '\"')
		{
			g_mini.quote.on_quote = 1;
			g_mini.quote.quote = g_mini.input[i];
			i = is_closed_quote(i);
			if (i == -1)
				return (-1);
		}
	}
	printf("nr_words: %i\n", nr_words);
	return (nr_words);
}

static char	*copy_word(int *input_i, int word_len)
{
	char	*arg;
	int		i;
	int		j;
	int		quote_len;

	arg = ft_calloc(word_len + 1, sizeof(char));
	if (!arg)
		return (NULL);
	while (g_mini.input[*input_i] == ' ')
		(*input_i)++;	
	i = *input_i;
	j = 0;
	while (word_len-- > 0)
	{
		if (g_mini.input[i] == '\'' || g_mini.input[i] == '\"')
		{
			g_mini.quote.quote = g_mini.input[i++];
			quote_len = 2;
			while (g_mini.input[i] != g_mini.quote.quote)
			{
				arg[j++] = g_mini.input[i++];
				quote_len++;
			}
			i++;
			word_len -= quote_len;
		}
		else
			arg[j++] = g_mini.input[i++];
	}
	*input_i = i;
	return (arg);
}



static int	count_word_length(int *input_i)
{
	int		word_len;
	int		i;

	i = *input_i;
	word_len = 0;
	ft_bzero(&g_mini.quote, sizeof(t_quote));
	while (g_mini.input[i] == ' ')
		i++;
	if (g_mini.input[i] == '\'' || g_mini.input[i] == '\"')
	{
		g_mini.quote.quote = g_mini.input[i];
		while (g_mini.input[++i] != g_mini.quote.quote)
		{
			if (g_mini.input[i] == '$' && g_mini.input[i + 1] != '?'
				&& g_mini.quote.quote != '\'')
				return (-1);
			word_len++;
		}
		i++;
	}
	else
	{
		while (g_mini.input[i]
			&& g_mini.input[i] != ' ' && !is_pipe(&g_mini.input[i]))
		{
			if (g_mini.input[i] == '$' && g_mini.input[i + 1] != '?')
				return (-1);
			word_len++;
			i++;
		}
	}
	printf("word_len: %i\n", word_len);
	return (word_len);
}

static void	copy_args(t_arguments **new_arg, int *input_i, int nr_words)
{
	int		word_len;
	int		wd;

	wd = -1;
	while (++wd < nr_words)
	{
		// ft_bzero(&g_mini.quote, sizeof(t_quote));
		word_len = count_word_length(input_i);
		if (word_len == -1)
			(*new_arg)->args[wd] = ft_expand_dollar(input_i);
		else
			(*new_arg)->args[wd] = copy_word(input_i, word_len);
	}
	
}

static int	split_args_and_add_pipe(t_arguments **new_arg, int *input_i)
{
	int	nr_words;

	if (is_pipe(&g_mini.input[*input_i]))
		(*new_arg)->pipe_type[0] = g_mini.input[(*input_i)++];
	if (g_mini.input[*input_i] == '<' || g_mini.input[*input_i] == '>')
		(*new_arg)->pipe_type[1] = g_mini.input[(*input_i)++];
	nr_words = count_words_until_pipe(*input_i);
	if (nr_words == -1)
		return (-2);
	(*new_arg)->args = ft_calloc(nr_words + 1, sizeof(char *));
	if (!((*new_arg)->args))
		return (-1);
	copy_args(new_arg, input_i, nr_words);
	//(*input_i)++;
	while (g_mini.input[*input_i] && g_mini.input[*input_i] == ' '
		&& !is_pipe(&g_mini.input[*input_i]))
		(*input_i)++;
	if (g_mini.input[*input_i])
		return (1);
	else
		return (0);
}

static t_arguments	*ft_parser(t_arguments *all_args, int input_i)
{
	t_arguments	*new_arg;
	int			has_another_arg;

	if (g_mini.input[input_i])
		new_arg = ft_calloc(1, sizeof(t_arguments));
	has_another_arg = split_args_and_add_pipe(&new_arg, &input_i);
	if (has_another_arg < 0)
	{
		ft_free_args(all_args);
		return (NULL);
	}
	add_to_end_of_the_list(&all_args, new_arg);
	if (has_another_arg)
		ft_parser(all_args, input_i);
	return (all_args);
}

t_arguments	*split_commands(t_arguments *old_argv)
{
	t_arguments	*all_args;

	if (old_argv)
		ft_free_args(old_argv);
	all_args = ft_parser(NULL, 0);
	return (all_args);
}
