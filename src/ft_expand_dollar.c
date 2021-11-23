/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand_dollar.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psleziak <psleziak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/25 14:18:10 by tosilva           #+#    #+#             */
/*   Updated: 2021/11/12 17:24:54 by psleziak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void
	copy_without_quotes(char *input, char *arg, int *input_i, int *arg_i)
{
	char	*content;
	int		content_len;

	content = NULL;
	*input_i += get_var_info(&input[*input_i + 1], &content, &content_len);
	ft_memcpy(&arg[*arg_i], content, content_len);
	*arg_i += content_len;
}

static int
	expanding_inside_quotes(char *input, int *arg_len, char *arg, bool copy)
{
	int		i;
	char	*content;
	int		content_len;

	content = NULL;
	i = 0;
	g_mini.quote.qt = input[i];
	while (input[++i] != g_mini.quote.qt)
	{
		if (is_shell_var(&input[i]) && g_mini.quote.qt != '\'')
		{
			i += get_var_info(&input[i + 1], &content, &content_len);
			if (copy)
				ft_memcpy(&arg[*arg_len], content, content_len);
			*arg_len += content_len;
		}
		else
		{
			if (copy)
				arg[*arg_len] = input[i];
			(*arg_len)++;
		}
	}
	ft_bzero((void *)&g_mini.quote, sizeof(t_quote));
	return (i);
}

static char	*ft_fill_arg(char *input, int arg_len)
{
	char	*arg;
	int		i;
	int		j;

	arg = ft_calloc(arg_len + 1, sizeof(char));
	if (!arg)
		return (NULL);
	i = -1;
	j = 0;
	while (j < arg_len)
	{
		++i;
		if (input[i] == '$')
			copy_without_quotes(input, arg, &i, &j);
		else if (!g_mini.quote.qt
			&& (input[i] == '\'' || input[i] == '\"'))
			i += expanding_inside_quotes(&input[i], &j, arg, 1);
		else
			arg[j++] = input[i];
	}
	return (arg);
}

static int	count_arg_len(int *i)
{
	int		arg_len;
	char	*content;
	int		content_len;

	content = NULL;
	arg_len = 0;
	while (g_mini.input[*i] && g_mini.input[*i] != g_mini.quote.qt
		&& g_mini.input[*i] != ' ' && !is_pipe(&g_mini.input[*i]))
	{
		if (g_mini.input[*i] == '$')
		{
			*i += get_var_info(&g_mini.input[*i + 1], &content, &content_len);
			arg_len += content_len;
		}
		else if (!g_mini.quote.qt
			&& (g_mini.input[*i] == '\'' || g_mini.input[*i] == '\"'))
			*i += expanding_inside_quotes(&g_mini.input[*i], &arg_len, NULL, 0);
		else
			arg_len++;
		*i += 1;
	}
	return (arg_len);
}

char	*ft_expand_dollar(int *input_i)
{
	char	*arg;
	int		arg_len;
	int		i;

	while (g_mini.input[*input_i] == ' ')
		(*input_i)++;
	arg_len = 0;
	i = *input_i;
	arg_len = count_arg_len(&i);
	arg = ft_fill_arg(&g_mini.input[*input_i], arg_len);
	if (g_mini.quote.qt)
		i++;
	*input_i = i;
	return (arg);
}
