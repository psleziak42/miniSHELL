/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_builtins.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psleziak <psleziak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/10 15:14:50 by bcosters          #+#    #+#             */
/*   Updated: 2021/11/10 17:24:23 by psleziak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * we must cover just "cd" withouth notihing, it goes to root
 * we must cover echoo, or any other wrong command so it displays "command not found:echoo" and exit code is 127
 *
*/
// TODO: join ~/Doc with full path
void	ft_cd(char **args, int fd_out) // we must cover just "cd" without nothing to go to root.
{
	char	oldpwd[4096];
	char	newpwd[4096];
	char	*key_n_content[2];
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
			ft_cmd_error_handler(args[0], NULL, "OLDPWD not set");
			return ;
		}
		chdir(temp);
	}
	else
	{
		if (chdir(args[1]) == -1)
			ft_cmd_error_handler(args[0], args[1], strerror(2));
	}
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

void	ft_export(char **args, int fd_out) //* Fully working
{
	t_list	*temp;
	char	**export_split;
	int		i;

	fd_out = (int)fd_out;
	if (!args[1])
	{
		ft_cmd_error_handler(args[0], NULL, "not enough arguments");
		return ;
	}
	temp = g_mini.env;
	i = 0;
	while (args[++i])
	{
		if (ft_strchr(args[i], '='))
		{
			export_split = ft_split(args[i], '=');
			if (!export_split[1])
				export_split[1] = ft_strdup("");
			if (ft_lstfind_update(&g_mini.env, export_split[1], export_split[0]) == 1)
			{
				free(export_split[0]);
				free(export_split[1]);
			}
			else
				ft_lstadd_back(&temp, ft_lstnew(export_split));
		}
	}
}

void	ft_unset(char **args, int fd_out)
{
	t_list	*temp;
	t_list	*previous;
	size_t	var_length;
	int		i;

	fd_out = (int)fd_out;
	if (!args[1])
	{
		ft_cmd_error_handler(args[0], NULL, "not enough arguments");
		return ;
	}
	i = 0;
	while (args[++i])
	{
		temp = g_mini.env;
		previous = g_mini.env;
		var_length = ft_strlen(args[i]);
		while (temp)
		{
			if (!ft_strncmp(temp->keyword, args[i], var_length + 1))
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
}

void	ft_env(char **args, int fd_out)
{
	t_list	*temp;

	if (args[1])
	{
		ft_cmd_error_handler(args[0], args[1], strerror(2));
		return ;
	}
	temp = g_mini.env;
	while (temp)
	{
		ft_putstr_fd(temp->keyword, fd_out);
		ft_putstr_fd("=", fd_out);
		ft_putendl_fd(temp->content, fd_out);
		temp = temp->next;
	}
}

void	ft_exit(char **args, int fd_out)
{
	int		exit_code;
	char	*temp_args[2];

	if (args[1])
		temp_args[0] = ft_strdup(args[1]);
	else
		temp_args[0] = NULL;
	if (args[1] && args[2])
		temp_args[1] = ft_strdup(args[2]);
	else
		temp_args[1] = NULL;
	exit_code = g_mini.exit_code;
	ft_putendl_fd("exit", fd_out);
	if (temp_args[0] && temp_args[1] && ft_isnumber(temp_args[0]))
		ft_cmd_error_handler("exit", NULL, "too many arguments");
	else
	{	
		ft_clear_data();
		if (temp_args[0] && ft_isnumber(temp_args[0]))
			exit_code = ft_atoi(temp_args[0]);
		else if (temp_args[0])
		{
			exit_code = 255;
			ft_cmd_error_handler("exit", temp_args[0], "numeric argument required");
		}
		ft_memdel((void**)&temp_args[0]);
		ft_memdel((void**)&temp_args[1]);
		exit(exit_code);
	}
}

void	ft_pwd(char **args, int fd_out)
{
	t_list	*temp;

	fd_out = (int)fd_out;
	args = (char **)args;
	temp = g_mini.env;
	while (temp)
	{
		if (!ft_strncmp(temp->keyword, "PWD", 3))
		{	
			ft_putendl_fd(temp->content, 1);
			break;
		}
		temp = temp->next;
	}
}

void	ft_echo(char **args, int fd_out)
{
	int i;

	i = 0;
	if (args[1] && !ft_strncmp(args[1], "-n", 3))
		i = 1;
	while (args[++i])
	{
		if (!ft_strncmp(args[i], "$?", 2))
			ft_putnbr_fd(g_mini.exit_code, fd_out);
		else
			ft_putstr_fd(args[i], fd_out);
		if (args[i + 1])
			ft_putstr_fd(" ", fd_out);
	}
	if (!args[1] || (args[1] && ft_strncmp(args[1], "-n", 3) != 0))
		ft_putendl_fd("", fd_out);
}

/*
 *	
*/

// void	ft_path(char **args, int fd_out)
// {
// 	int		i;
// 	int		child_id;
// 	char	*strjoin;
// 	char	*strjoin2;

// 	i = -1;
// 	child_id = fork();
// 	if (child_id == -1)
// 		printf("Error FORK\n");
// 	else if (child_id == 0)
// 	{
// 		while (g_mini.path[++i])
// 		{
// 			strjoin = ft_strjoin(g_mini.path[i], "/");
// 			strjoin2 = ft_strjoin(strjoin, args[0]);
// 			if (execve(strjoin2, &args[0], NULL))
// 				;
// 			free(strjoin); // this wont be freed if because execve kills the process
// 			free(strjoin2);
// 		}
// 		ft_error_exit("error execve");
// 	}
// 	else
// 	{
// 		wait(NULL);
// 		/*if (strjoin)
// 			free(strjoin);
// 		if (strjoin2)
// 			free(strjoin2);*/
// 	}
// }
