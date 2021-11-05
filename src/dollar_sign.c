/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar_sign.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tony <tony@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/26 16:31:57 by tosilva           #+#    #+#             */
/*   Updated: 2021/11/02 00:46:36 by tony             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Every strjoin call needs a free
**/
void	ft_dollar_sign(char *argv)
{
	t_list	*temp;

	temp = g_mini.env;
	while (temp)
	{
		if (!ft_strncmp(temp->keyword, argv, ft_strlen(argv)))
		{
			ft_putstr_fd(temp->content, 1);
			break ;
		}
		temp = temp->next;
	}
}

/*
**	STILL NEED TO IMPLEMENT: a way to print what is before the $
	=> EXAMPLE: ben$PATH => ben + expanded PATH
	=> Only in "" !
*/
void	ft_expand_var(char *argv)
{
	//int	i;

	//i = -1;
	if (argv[0] == '?')
	{
		ft_putnbr_fd(g_mini.exit_code, g_mini.fd);
		return ;
	}
	ft_dollar_sign(argv);
}
