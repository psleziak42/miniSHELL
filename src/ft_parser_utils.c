/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parser_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psleziak <psleziak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/11 17:03:23 by psleziak          #+#    #+#             */
/*   Updated: 2021/11/11 19:22:21 by psleziak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_pipe(char *str)
{
	return (str[0] == '|'
		|| str[0] == '<'
		|| (str[0] == '<' && str[1] == '<')
		|| str[0] == '>'
		|| (str[0] == '>' && str[1] == '>'));
}

int	is_closed_quote(int input_i)
{
	int	i;

	i = input_i;
	while (g_mini.input[++i])
	{
		if (g_mini.input[i] == g_mini.quote.qt)
		{
			g_mini.quote.on_quote = 0;
			break ;
		}
	}
	if (g_mini.quote.on_quote)
	{
		deflt_err_handler(strerror(22), "quotes must be closed", TOKEN_ERR);
		return (-1);
	}
	return (i);
}

int	copy_word_inside_quote(char *arg, char *str, int *i, int *word_len)
{
	int	quote_len;
	int	j;

	g_mini.quote.qt = str[(*i)++];
	quote_len = 0;
	j = 0;
	while (str[*i] != g_mini.quote.qt)
	{
		arg[j++] = str[(*i)++];
		quote_len++;
	}
	(*i)++;
	*word_len -= (quote_len - 1);
	return (j);
}
