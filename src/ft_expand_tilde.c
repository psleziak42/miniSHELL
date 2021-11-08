/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand_tilde.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psleziak <psleziak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/08 17:29:54 by psleziak          #+#    #+#             */
/*   Updated: 2021/11/08 18:49:54 by psleziak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


static char	*copy_word(char *str, int word_len)
{
	char	*arg;
	int		i;
	int		j;
	int		quote_len;

	arg = ft_calloc(word_len + 1, sizeof(char));
	if (!arg)
		return (NULL);
	i = 0;
	while (str[i] == ' ')
		i++;	
	j = 0;
	while (word_len--)
	{
		if (str[i] == '\'' || str[i] == '\"')
		{
			g_mini.quote.quote = str[i++];
			quote_len = 0;
			while (str[i] != g_mini.quote.quote)
			{
				arg[j++] = str[i++];
				quote_len++;
			}
			i++;
			word_len -= (quote_len + 1);
		}
		else
			arg[j++] = str[i++];
	}
	return (arg);
}

char	*ft_expand_tilde(int *input_i)
{
	char	*arg;
	char	*without_tilde;
	char	*expanded_str;
	int	 i;

	while (g_mini.input[*input_i] == ' ')
		(*input_i)++;
	expanded_str = ft_strdup(&g_mini.input[*input_i]);
	i = -1;
	while (expanded_str[++i] && !is_pipe(&expanded_str[i]) && expanded_str[i] != ' ')
	{
		if ((i == 0 || (i > 0 && expanded_str[i - 1] == ' ')) && expanded_str[i] == '~'
			&& (expanded_str[i + 1] == '\0' || expanded_str[i + 1] == '/' || expanded_str[i + 1] == ' '))
		{
			without_tilde = ft_strjoin(getenv("HOME"), &expanded_str[i + 1]);
			free(expanded_str);
			expanded_str = without_tilde;
		}
	}
	arg = copy_word(expanded_str, i);
	free(expanded_str);
	while (g_mini.input[*input_i] && !is_pipe(&g_mini.input[*input_i]) && g_mini.input[*input_i] != ' ')
		(*input_i)++;
	return (arg);
}
