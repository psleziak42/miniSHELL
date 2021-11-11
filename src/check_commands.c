/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_commands.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psleziak <psleziak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/28 16:38:52 by tosilva           #+#    #+#             */
/*   Updated: 2021/11/11 19:09:21 by psleziak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_cmd_w_path(char *cmd, char *path)
{
	char	*cmd_w_path;
	char	*temp;

	if (ft_strchr(cmd, '/'))
		cmd_w_path = ft_strdup(cmd);
	else
	{
		temp = ft_strjoin(path, "/");
		cmd_w_path = ft_strjoin(temp, cmd);
		free(temp);
	}
	return (cmd_w_path);
}

static char	*check_n_get_cmd_path(char *cmd)
{
	char	*cmd_w_path;
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
		cmd_w_path = get_cmd_w_path(cmd, g_mini.path[i]);
		if (access(cmd_w_path, F_OK | X_OK) == 0)
			return (cmd_w_path);
		free(cmd_w_path);
	}
	cmd_error_handler(cmd, NULL, INVALID_COMMAND, CMD_NOT_FOUND);
	return (NULL);
}

static void	poland_independence_day(t_arguments *lst_args, char *temp,
				char *full_file_path, char *buf)
{
	if (get_type_of_pipe(lst_args->pipe_type) == 1)
	{
		temp = ft_strjoin(getcwd(buf, 4095), "/");
		full_file_path = ft_strjoin(temp, lst_args->args[0]);
		free(temp);
		if (access(full_file_path, F_OK | R_OK) == 0)
		{
			free(full_file_path);
			if (lst_args->args[1])
				lst_args->cmd_w_path = check_n_get_cmd_path(lst_args->args[1]);
			else
				lst_args->is_valid = 1;
		}
		else
			cmd_error_handler(lst_args->args[0], 0, strerror(2), GENERAL_ERR);
	}
	else if (get_type_of_pipe(lst_args->pipe_type) == 2)
	{
		if (lst_args->args[1])
			lst_args->cmd_w_path = check_n_get_cmd_path(lst_args->args[1]);
		else
			lst_args->is_valid = 1;
	}
	else if (get_type_of_pipe(lst_args->pipe_type) >= 3)
		lst_args->is_valid = 1;
}

static void	init_check_commands(t_arguments	**lst_args, char **temp,
				char **full_file_path)
{
	*temp = NULL;
	*full_file_path = NULL;
	*lst_args = g_mini.argv;
}

void	check_commands(void)
{
	t_arguments	*lst_args;
	char		*temp;
	char		*full_file_path;
	char		buf[4096];

	init_check_commands(&lst_args, &temp, &full_file_path);
	while (lst_args)
	{
		if (!lst_args->args[0])
		{
			if (lst_args->pipe_type[0] == '|' && lst_args->pipe_type[1] == 0)
				deflt_err_handler(SYNTAX_ERROR, lst_args->pipe_type, TOKEN_ERR);
			else
				deflt_err_handler(SYNTAX_ERROR, "'newline'", TOKEN_ERR);
			ft_free_args(g_mini.argv);
			return ;
		}
		else if (is_redirection(lst_args->pipe_type))
			poland_independence_day(lst_args, temp, full_file_path, buf);
		else if (lst_args->args[0])
			lst_args->cmd_w_path = check_n_get_cmd_path(lst_args->args[0]);
		if (lst_args->cmd_w_path)
			lst_args->is_valid = 1;
		lst_args = lst_args->next;
	}
}
