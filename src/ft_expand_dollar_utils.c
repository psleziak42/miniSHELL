/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand_dollar_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psleziak <psleziak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/26 16:35:56 by tosilva           #+#    #+#             */
/*   Updated: 2021/11/12 15:26:47 by psleziak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_shell_var(char *var)
{
	return (var[0] == '$' && var[1] && ft_isalpha(var[1]));
}

static int	is_end_character(char c)
{
	return (!ft_isalpha(c));
	//  c == ' '
	// 	|| c == '\'' || c == '\"'
	// 	|| c == '|'
	// 	|| c == '<' || c == '>'
	// 	|| c == '\0');
}

static int	ft_keyword_length(char *input)
{
	int		length;

	length = 0;
	while (!is_end_character(input[length]))
		length++;
	return (length);
}

static char	*get_content_from_keyword(char *input, int length)
{
	t_list	*temp;
	char	old_chr;

	old_chr = input[length];
	input[length] = 0;
	temp = g_mini.env;
	while (temp->next)
	{
		if (ft_strncmp(input, temp->keyword, length + 1) == 0)
		{
			input[length] = old_chr;
			return (temp->content);
		}
		temp = temp->next;
	}
	input[length] = old_chr;
	return (NULL);
}

// static int	ft_get_countent_length(char *input, int kw_length)
// {
// 	char	*shell_variable;

// 	shell_variable = get_content_from_keyword(input, kw_length);
// 	if (!shell_variable)
// 		return (0);
// 	return (ft_strlen(shell_variable));
// }

int	get_var_info(char *var_name, char **kw_content, int *content_len)
{
	int		keyword_len;

	keyword_len = ft_keyword_length(var_name);
	*kw_content = get_content_from_keyword(var_name, keyword_len);
	if (content_len)
		*content_len = ft_strlen(*kw_content);
	return (keyword_len);
}
