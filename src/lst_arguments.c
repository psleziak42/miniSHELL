/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_arguments.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psleziak <psleziak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/25 15:29:49 by tosilva           #+#    #+#             */
/*   Updated: 2021/11/10 18:03:18 by psleziak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_arguments	*ft_lstlast_arg(t_arguments *lst)
{
	t_arguments	*previous;
	t_arguments	*current;

	previous = lst;
	current = lst;
	if (lst == NULL)
		return (lst);
	while (current != NULL)
	{
		if (current->next == NULL)
			return (current);
		previous = current;
		current = current->next;
	}
	return (previous);
}

void	add_to_end_of_the_list(t_arguments **all_args, t_arguments *new_arg)
{
	if (*all_args == NULL)
	{
		new_arg->next = *all_args;
		*all_args = new_arg;
	}
	else
		ft_lstlast_arg(*all_args)->next = new_arg;
}

void	ft_free_args(t_arguments *old_argv)
{
	int			i;
	t_arguments	*temp;

	while (old_argv)
	{
		temp = old_argv->next;
		i = -1;
		while (old_argv->args[++i])
			ft_strdel(&(old_argv->args[i]));
		ft_strdel(old_argv->args);
		ft_strdel(&(old_argv->cmd_w_path));
		free(old_argv);
		old_argv = temp;
	}
	g_mini.argv = NULL;
}
