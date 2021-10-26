/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand_dollar.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosilva <tosilva@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/25 14:18:10 by tosilva           #+#    #+#             */
/*   Updated: 2021/10/25 18:14:42 by tosilva          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_end_character(char c)
{
	return (c == ' '
			|| c == '\'' || c == '\"'
			|| c == '|'
			|| c == '<' || c == '>'
			|| c == '\0');
}

static int	ft_keyword_length(char *input)
{
	int		length;

	length = 0;
	while (!is_end_character(input[length]))
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

static char	*ft_fill_arg(char *input, int arg_len)
{
	char	*arg;
	char	*kw_content;
	int		keyword_len;
	int		i;
	int		j;

	arg = ft_calloc(arg_len + 1, sizeof(char));
	if (!arg)
		return (0);
	i = -1;
	j = 0;
	while (input[++i] != g_mini.quote.quote && input[i])
	{
		if (input[i] == '$')
		{
			keyword_len = ft_keyword_length(&input[i + 1]);
			kw_content = get_content_from_keyword(&input[i + 1], keyword_len); 
			ft_memcpy(&arg[j], kw_content, ft_strlen(kw_content));
			i += keyword_len;
			j += ft_strlen(kw_content);
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
					keyword_len = ft_keyword_length(&input[i + 1]);
					kw_content = get_content_from_keyword(&input[i + 1], keyword_len); 
					ft_memcpy(&arg[j], kw_content, ft_strlen(kw_content));
					i += keyword_len;
					j += ft_strlen(kw_content);
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

char	*ft_expand_dollar(int *input_i)
{
	int		j;
	int		arg_len;
	int		keyword_len;
	char	*arg;

	arg_len = 0;
	while (g_mini.input[*input_i] == ' '
		|| g_mini.input[*input_i] == g_mini.quote.quote)
		(*input_i)++;
	j = *input_i;
	while (g_mini.input[j] && g_mini.input[j] != g_mini.quote.quote)
	{
		if (g_mini.input[j] == '$')
		{
			keyword_len = ft_keyword_length(&g_mini.input[j + 1]);
			arg_len += ft_get_countent_length(&g_mini.input[j + 1], keyword_len);
			j += keyword_len;
		}
		else if (!g_mini.quote.quote
			&& (g_mini.input[j] == '\'' || g_mini.input[j] == '\"'))
		{
			g_mini.quote.quote = g_mini.input[j];
			while (g_mini.input[++j] != g_mini.quote.quote)
			{
				if (g_mini.input[j] == '$' && g_mini.input[j + 1] != '?'
					&& g_mini.quote.quote != '\'')
				{
					keyword_len = ft_keyword_length(&g_mini.input[j + 1]);
					arg_len += ft_get_countent_length(&g_mini.input[j + 1], keyword_len);
					j += keyword_len;
				}
				else
					arg_len++;
			}
			//j++;
			ft_bzero(&g_mini.quote, sizeof(t_quote));
		}
		else
			arg_len++;
		j++;
	}
	arg = ft_fill_arg(&g_mini.input[*input_i], arg_len);
	if (g_mini.quote.quote)
		j++;
	*input_i = j;
	return (arg);
}
