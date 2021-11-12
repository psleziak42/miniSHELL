/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_n_unset.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psleziak <psleziak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/11 15:46:45 by tosilva           #+#    #+#             */
/*   Updated: 2021/11/12 21:59:07 by psleziak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	find_and_update_env_var(char *env_var)
{
	char	**export_split;

	if (ft_strchr(env_var, '='))
	{
		export_split = ft_split(env_var, '=');
		if (!export_split[1])
			export_split[1] = ft_strdup("");
		if (ft_lstfind_update(&g_mini.env, export_split[1],
				export_split[0]) == 1)
		{
			free(export_split[0]);
			free(export_split[1]);
		}
		else
			ft_lstadd_back(&g_mini.env, ft_lstnew(export_split));
	}
}

/*
* cmd_error_handler(args[0], NULL, FEW_ARGS, MISUSE_BUILTIN);
*/
void	ft_export(char **args, int fd_out)
{
	int		i;

	fd_out = (int)fd_out;
	if (!args[1])
	{
		ft_env(args, fd_out);
		return ;
	}
	i = 0;
	while (args[++i])
	{
		if (!ft_isalpha(args[i][0]) && args[i][0] != '_')
			cmd_error_handler(args[0], args[i],
				"not a valid identifier", GENERAL_ERR);
		else
			find_and_update_env_var(args[i]);
	}
}

static void	find_and_delete_env_var(char *keyword)
{
	t_list	*temp;
	t_list	*previous;
	size_t	var_length;

	temp = g_mini.env;
	previous = g_mini.env;
	var_length = ft_strlen(keyword);
	while (temp)
	{
		if (!ft_strncmp(temp->keyword, keyword, var_length + 1))
		{
			if (temp == g_mini.env)
				g_mini.env = temp->next;
			else
				previous->next = temp->next;
			ft_lstdelone(temp, &free);
			break ;
		}
		if (temp != g_mini.env)
			previous = previous->next;
		temp = temp->next;
	}
}

void	ft_unset(char **args, int fd_out)
{
	int		i;

	fd_out = (int)fd_out;
	if (!args[1])
		return ;
		//cmd_error_handler(args[0], NULL, FEW_ARGS, MISUSE_BUILTIN);
	i = 0;
	while (args[++i])
	{
		if (!ft_isalpha(args[i][0]) && args[i][0] != '_')
			cmd_error_handler(args[0], args[i],
				"not a valid identifier", GENERAL_ERR);
		else
			find_and_delete_env_var(args[i]);
	}
}
