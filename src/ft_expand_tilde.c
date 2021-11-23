/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand_tilde.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psleziak <psleziak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/08 17:29:54 by psleziak          #+#    #+#             */
/*   Updated: 2021/11/11 18:36:09 by psleziak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*copy_word(char *str, int word_len)
{
	char	*arg;
	int		i;
	int		j;

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
			j += copy_word_inside_quote(&arg[j], str, &i, &word_len);
		else
			arg[j++] = str[i++];
	}
	return (arg);
}

/* 
*	we did i - 2 in tilde, to be able to use copy_word_inside_quote with
*	word_len -= (quote_len -1);
*	before it was word_len -= (quote_len + 1);
*/
char	*ft_expand_tilde(int *input_i)
{
	char	*arg;
	char	*without_tilde;
	char	*expanded_str;
	int		i;

	ft_skip_spaces(input_i);
	expanded_str = ft_strdup(&g_mini.input[*input_i]);
	i = -1;
	while (expanded_str[++i] && !is_pipe(&expanded_str[i])
		&& expanded_str[i] != ' ')
	{
		if (is_tilde_to_home(i, expanded_str))
		{
			without_tilde = ft_strjoin(getenv("HOME"), &expanded_str[i + 1]);
			free(expanded_str);
			expanded_str = without_tilde;
		}
	}
	if (ft_strchr(expanded_str, '\"') || ft_strchr(expanded_str, '\''))
		i -= 2;
	arg = copy_word(expanded_str, i);
	free(expanded_str);
	ft_go_to_next_cmd(input_i);
	return (arg);
}
