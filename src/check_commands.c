/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_commands.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psleziak <psleziak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/28 16:38:52 by tosilva           #+#    #+#             */
/*   Updated: 2021/10/30 19:03:15 by psleziak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*check_n_get_cmd_path(char *cmd)
{
	char	*temp;
	char	*full_cmd_path;
	int		i;

	i = -1;
	while (g_mini.builtins.cmd[++i])
	{
		if (ft_strncmp(g_mini.builtins.cmd[i], cmd, ft_strlen(cmd)) == 0)
			return (ft_strdup(cmd));
	}
	i = -1;
	while (g_mini.path[++i])
	{
		temp = ft_strjoin(g_mini.path[i], "/");
		full_cmd_path = ft_strjoin(temp, cmd);
		free(temp);
		if (access(full_cmd_path, F_OK | X_OK) == 0)
			return (full_cmd_path);
		free(full_cmd_path);
	}
	// TODO error with command name
	ft_error_handler("Command not found"); //? to verify if errno set message automatically 
	return (NULL);
}

void	check_commands(void)
{
	t_arguments	*list_args;
	char		*temp;
	char		*full_file_path;
	char		buf[4096];

	list_args = g_mini.argv;
	while (list_args)
	{
		if (get_type_of_pipe(list_args->pipe_type) == 1)
		{
			temp = ft_strjoin(getcwd(buf, 4095), "/");
			full_file_path = ft_strjoin(temp, list_args->args[0]);
			free(temp);
			if (access(full_file_path, F_OK | R_OK) == 0)
			{
				free(full_file_path);
				if (list_args->args[1])
					list_args->full_arg_path
						= check_n_get_cmd_path(list_args->args[1]);
				else
					list_args->is_valid = 1;
			}
			else
				ft_error_handler(NULL); //? to verify if errno set message automatically 
		}
		else if (get_type_of_pipe(list_args->pipe_type) == 2)
		{
			if (list_args->args[1])
				list_args->full_arg_path = check_n_get_cmd_path(list_args->args[1]);
			else
				list_args->is_valid = 1;
		}
		else if (get_type_of_pipe(list_args->pipe_type) >= 3)
			list_args->is_valid = 1;
		else if (list_args->args[0])
			list_args->full_arg_path = check_n_get_cmd_path(list_args->args[0]);
		if (list_args->full_arg_path)
			list_args->is_valid = 1;
		list_args = list_args->next;
	}
}
