/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand_dollar.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosilva <tosilva@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/25 14:18:10 by tosilva           #+#    #+#             */
/*   Updated: 2021/10/26 17:32:07 by tosilva          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*ft_fill_arg(char *input, int arg_len)
{
	char	*arg;
	char	*kw_content;
	int		content_len;
	int		i;
	int		j;

	arg = ft_calloc(arg_len + 1, sizeof(char));
	if (!arg)
		return (NULL);
	kw_content = NULL;
	i = -1;
	j = 0;
	while (j < arg_len)
	{
		++i;
		if (input[i] == '$')
		{
			i += get_var_info(&input[i + 1], &kw_content, &content_len);
			ft_memcpy(&arg[j], kw_content, content_len);
			j += content_len;
		}
		else if (!g_mini.quote.quote
			&& (input[i] == '\'' || input[i] == '\"'))
		{
			g_mini.quote.quote = input[i];
			while (input[++i] != g_mini.quote.quote)
			{
				if (input[i] == '$' && input[i + 1] != '?'
					&& g_mini.quote.quote != '\'')
				{
					i += get_var_info(&input[i + 1], &kw_content, &content_len);
					ft_memcpy(&arg[j], kw_content, content_len);
					j += content_len;
				}
				else
					arg[j++] = input[i];
			}
			ft_bzero(&g_mini.quote, sizeof(t_quote));
		}
		else
			arg[j++] = input[i];
	}
	return (arg);
}

static int	count_arg_len(int *i)
{
	int		arg_len;
	char	*kw_content;
	int		content_len;

	kw_content = NULL;
	arg_len = 0;
	while (g_mini.input[*i] && g_mini.input[*i] != g_mini.quote.quote
		&& g_mini.input[*i] != ' ' && !is_pipe(&g_mini.input[*i]))
	{
		if (g_mini.input[*i] == '$')
		{
			*i += get_var_info(&g_mini.input[*i + 1], &kw_content, &content_len);
			arg_len += content_len;
		}
		else if (!g_mini.quote.quote
			&& (g_mini.input[*i] == '\'' || g_mini.input[*i] == '\"'))
		{
			g_mini.quote.quote = g_mini.input[*i];
			while (g_mini.input[++(*i)] != g_mini.quote.quote)
			{
				if (g_mini.input[*i] == '$' && g_mini.input[*i + 1] != '?'
					&& g_mini.quote.quote != '\'')
				{
					*i += get_var_info(&g_mini.input[*i + 1],
							&kw_content, &content_len);
					arg_len += content_len;
				}
				else
					arg_len++;
			}
			ft_bzero((void *)&g_mini.quote, sizeof(t_quote));
		}
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
	if (g_mini.quote.quote)
		i++;
	*input_i = i;
	return (arg);
}
