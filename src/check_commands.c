/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_commands.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psleziak <psleziak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/28 16:38:52 by tosilva           #+#    #+#             */
/*   Updated: 2021/11/10 15:20:03 by psleziak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*check_n_get_cmd_path(char *cmd)
{
	char	*temp;
	char	*full_cmd_path;
	size_t	cmd_length;
	int		i;

	cmd_length = ft_strlen(cmd);
	i = -1;
	while (g_mini.builtins.cmd[++i])
	{
		if (ft_strncmp(g_mini.builtins.cmd[i], cmd, cmd_length + 1) == 0)
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
	ft_cmd_error_handler(cmd, NULL, INVALID_COMMAND);
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
		if (!list_args->args[0])
		{
			if (list_args->pipe_type[0] == '|' && list_args->pipe_type[1] == '\0')
				ft_default_error_handler("Syntax error near unexpected token", list_args->pipe_type);
			else
				ft_default_error_handler("Syntax error near unexpected token", "'newline'");
			ft_free_args(g_mini.argv);
			return ;
		}
		else if (get_type_of_pipe(list_args->pipe_type) == 1)
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
				ft_cmd_error_handler(list_args->args[0], NULL, strerror(2));
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
