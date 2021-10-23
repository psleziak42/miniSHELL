/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parser.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosilva <tosilva@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/23 16:21:54 by tosilva           #+#    #+#             */
/*   Updated: 2021/10/23 19:15:42 by tosilva          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_pipe(char *str)
{
	return (str[0] == '|'
		|| str[0] == '<'
		|| str[0] == '>');
}

static int	count_words_until_pipe(int input_i)
{
	int	nr_words;
	int	i;

	nr_words = 0;
	i = input_i - 1;
	while (!is_pipe(g_mini.input[++i]) || g_mini.input[i])
	{
		if (!nr_words
		|| (g_mini.input[i] == ' ' && g_mini.input[i + 1] != ' '))
			nr_words++;
		else if (g_mini.input[i] == '\'' || g_mini.input[i] == '\"')
		{
			g_mini.quote.on_quote = 1;
			g_mini.quote.quote = g_mini.input[i];
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
				ft_error_handler("quotes must be closed");
				return (-1);
			}
		}
	}
	return (nr_words);
}

static char	*copy_args(int *input_i, int *has_another_arg)
{
	
}

static int	split_args_and_add_pipe(t_arguments **new_arg, int *input_i)
{
	int	nr_words;
	int	has_another_arg;

	if (is_pipe(g_mini.input[*input_i]))
		(*new_arg)->pipe_type = g_mini.input[(*input_i)++];
	nr_words = count_words_until_pipe(input_i);
	(*new_arg)->args = ft_calloc(nr_words + 1, sizeof(char));
	if (!((*new_arg)->args))
		return (NULL);
	(*new_arg)->args = copy_args(input_i, &has_another_arg);
	return (has_another_arg);
}

static t_arguments	*ft_parser(t_arguments *all_args, int input_i)
{
	t_arguments	*new_arg;
	int			has_another_arg;

	if (g_mini.input[input_i])
		new_arg = ft_calloc(1, sizeof(t_arguments));
	has_another_arg = split_args_and_add_pipe(&new_arg, &input_i);
	add_arg_to_end(all_args, new_arg);
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
