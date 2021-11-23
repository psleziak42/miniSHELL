/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand_tilde_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psleziak <psleziak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/11 15:04:10 by psleziak          #+#    #+#             */
/*   Updated: 2021/11/11 15:04:45 by psleziak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_skip_spaces(int *input_i)
{
	while (g_mini.input[*input_i] == ' ')
		(*input_i)++;
}

void	ft_go_to_next_cmd(int *input_i)
{
	while (g_mini.input[*input_i] && !is_pipe(&g_mini.input[*input_i])
		&& g_mini.input[*input_i] != ' ')
		(*input_i)++;
}

bool	is_tilde_to_home(int i, char *str)
{
	return ((i == 0 || (i > 0 && str[i - 1] == ' '))
		&& str[i] == '~' && (str[i + 1] == '\0'
			|| str[i + 1] == '/' || str[i + 1] == ' '));
}
