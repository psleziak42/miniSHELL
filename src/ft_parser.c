/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parser.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psleziak <psleziak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/23 16:21:54 by tosilva           #+#    #+#             */
/*   Updated: 2021/11/11 17:27:41 by psleziak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	check_and_save_redirection_or_pipe(t_arguments *temp, int *input_i)
{
	int	pipe_i;

	pipe_i = 0;
	if (is_pipe(&g_mini.input[*input_i]))
		temp->pipe_type[pipe_i++] = g_mini.input[(*input_i)++];
	ft_skip_spaces(input_i);
	if (g_mini.input[*input_i] == '<' || g_mini.input[*input_i] == '>')
	{
		if (g_mini.input[*input_i] != g_mini.input[*input_i + 1])
			temp->pipe_type[pipe_i++] = g_mini.input[(*input_i)++];
		else
		{
			temp->pipe_type[pipe_i++] = g_mini.input[(*input_i)++];
			temp->pipe_type[pipe_i++] = g_mini.input[(*input_i)++];
		}
	}
}

static int	count_words_until_pipe(int input_i)
{
	int	nr_words;
	int	i;

	nr_words = 0;
	i = input_i - 1;
	while (g_mini.input[++i] && !is_pipe(&g_mini.input[i]))
	{
		if (!nr_words)
			nr_words++;
		if (g_mini.input[i] == ' ' && g_mini.input[i + 1] != ' '
			&& !is_pipe(&g_mini.input[i + 1]))
			nr_words++;
		else if (g_mini.input[i] == '\'' || g_mini.input[i] == '\"')
		{
			g_mini.quote.on_quote = 1;
			g_mini.quote.qt = g_mini.input[i];
			i = is_closed_quote(i);
			if (i == -1)
				return (-1);
		}
	}
	return (nr_words);
}

static int	split_args_and_add_pipe(t_arguments **new_arg, int *input_i)
{
	int	nr_words;

	check_and_save_redirection_or_pipe(*new_arg, input_i);
	ft_skip_spaces(input_i);
	nr_words = count_words_until_pipe(*input_i);
	if (nr_words == -1)
		return (-2);
	(*new_arg)->args = ft_calloc(nr_words + 1, sizeof(char *));
	if (!((*new_arg)->args))
		return (-1);
	copy_args(new_arg, input_i, nr_words);
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
