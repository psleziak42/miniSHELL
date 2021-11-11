/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psleziak <psleziak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/11 15:44:55 by tosilva           #+#    #+#             */
/*   Updated: 2021/11/11 18:16:39 by psleziak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	update_old_n_new_pwd(char *oldpwd)
{
	char	newpwd[4096];
	char	*key_n_content[2];

	if (ft_lstfind_content(&g_mini.env, "OLDPWD"))
		ft_lstfind_update(&g_mini.env, oldpwd, "OLDPWD");
	else
	{
		key_n_content[0] = ft_strdup("OLDPWD");
		key_n_content[1] = ft_strdup(oldpwd);
		ft_lstadd_back(&g_mini.env, ft_lstnew(key_n_content));
	}
	getcwd(newpwd, 4096);
	ft_lstfind_update(&g_mini.env, newpwd, "PWD");
}

void	ft_cd(char **args, int fd_out)
{
	char	oldpwd[4096];
	char	*temp;

	fd_out = (int)fd_out;
	getcwd(oldpwd, 4096);
	if (!ft_strncmp(args[0], "cd", 3) && !args[1])
		chdir(ft_lstfind_content(&g_mini.env, "HOME"));
	else if (args[1][0] == '-')
	{
		temp = ft_lstfind_content(&g_mini.env, "OLDPWD");
		if (!temp)
		{
			cmd_error_handler(args[0], NULL, "OLDPWD not set", GENERAL_ERR);
			return ;
		}
		chdir(temp);
	}
	else
	{
		if (chdir(args[1]) == -1)
			cmd_error_handler(args[0], args[1], strerror(2), GENERAL_ERR);
	}
	update_old_n_new_pwd(oldpwd);
}
