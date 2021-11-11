/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parser_copy_args.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psleziak <psleziak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/11 17:25:51 by tosilva           #+#    #+#             */
/*   Updated: 2021/11/11 18:37:32 by psleziak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*copy_word(int *input_i, int word_len)
{
	char	*arg;
	int		i;
	int		j;

	arg = ft_calloc(word_len + 1, sizeof(char));
	if (!arg)
		return (NULL);
	ft_skip_spaces(input_i);
	i = *input_i;
	j = 0;
	while (word_len--)
	{
		if (g_mini.input[i] == '\'' || g_mini.input[i] == '\"')
			j += copy_word_inside_quote(&arg[j], g_mini.input, &i, &word_len);
		else
			arg[j++] = g_mini.input[i++];
	}
	if (g_mini.input[i]
		&& (g_mini.input[i] == '\'' || g_mini.input[i] == '\"'))
		i += 2;
	*input_i = i;
	return (arg);
}

static int	count_word_len_inside_quotes(int *i, int word_len)
{
	g_mini.quote.qt = g_mini.input[*i];
	while (g_mini.input[++(*i)] != g_mini.quote.qt)
	{
		if (is_shell_var(&g_mini.input[*i]) && g_mini.quote.qt != '\'')
			return (-1);
		word_len++;
	}
	(*i)++;
	return (word_len);
}

static int	count_word_length(int *input_i)
{
	int		word_len;
	int		i;

	i = *input_i;
	word_len = 0;
	ft_bzero(&g_mini.quote, sizeof(t_quote));
	ft_skip_spaces(&i);
	while (g_mini.input[i]
		&& g_mini.input[i] != ' ' && !is_pipe(&g_mini.input[i]))
	{
		if (g_mini.input[i] == '\'' || g_mini.input[i] == '\"')
			word_len = count_word_len_inside_quotes(&i, word_len);
		else
		{
			if (is_shell_var(&g_mini.input[i]))
				return (-1);
			if (is_tilde_to_home(i, g_mini.input))
				return (-2);
			word_len++;
			i++;
		}
		if (word_len == -1)
			return (-1);
	}
	return (word_len);
}

void	copy_args(t_arguments **new_arg, int *input_i, int nr_words)
{
	int		word_len;
	int		wd;

	wd = -1;
	while (++wd < nr_words)
	{
		word_len = count_word_length(input_i);
		ft_bzero(&g_mini.quote, sizeof(t_quote));
		if (word_len == -1)
			(*new_arg)->args[wd] = ft_expand_dollar(input_i);
		else if (word_len == -2)
			(*new_arg)->args[wd] = ft_expand_tilde(input_i);
		else
			(*new_arg)->args[wd] = copy_word(input_i, word_len);
	}
}
