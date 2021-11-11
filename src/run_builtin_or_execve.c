/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_builtin_or_execve.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psleziak <psleziak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/11 13:36:21 by psleziak          #+#    #+#             */
/*   Updated: 2021/11/11 19:53:54 by psleziak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	run_builtin(char **args, int fd_out)
{
	int	i;
	int	var_len;

	var_len = ft_strlen(args[0]);
	i = -1;
	while (g_mini.builtins.cmd[++i])
	{
		if (!ft_strncmp(g_mini.builtins.cmd[i], args[0], var_len + 1))
		{
			g_mini.builtins.builtin_func[i](args, fd_out);
			if (g_mini.argv->next != NULL)
				exit(EXIT_SUCCESS);
			else
				return (0);
		}
	}
	return (1);
}

static void	run_execve(char *cmd_w_path, char **args, int fd_in, int fd_out)
{
	int	process;

	process = 0;
	if (g_mini.argv->next == NULL)
		process = fork();
	if (process == -1)
		error_exit("Fork error", strerror(10), GENERAL_ERR);
	else if (process == 0)
	{
		dup2(fd_in, STDIN_FILENO);
		dup2(fd_out, STDOUT_FILENO);
		if (fd_in != STDIN_FILENO)
			close(fd_in);
		if (fd_out != STDOUT_FILENO)
			close(fd_out);
		execve(cmd_w_path, args, NULL);
		exit(EXIT_FAILURE);
	}
	else
	{
		g_mini.pid_id = process;
		wait(NULL);
		g_mini.pid_id = 0;
	}
	// cmd_error_handler(args[0], NULL, INVALID_COMMAND, CMD_NOT_FOUND);
}

void	
	run_builtin_or_execve(char *cmd_w_path, char **args, int fd_in, int fd_out)
{
	if (run_builtin(args, fd_out))
		run_execve(cmd_w_path, args, fd_in, fd_out);
}
