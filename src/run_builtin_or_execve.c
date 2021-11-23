/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_builtin_or_execve.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psleziak <psleziak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/11 13:36:21 by psleziak          #+#    #+#             */
/*   Updated: 2021/11/12 18:48:48 by psleziak         ###   ########.fr       */
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

/*
* If there is a single command /line48/ then we create fork and 
* everything after waitpid /line68/ is to take exit code from that fork.
*
* When we enter here from pipe, we create fork there in the pipe. 
* We set here process to 0 so if line48 is false (so there is next link in 
* list) we do not create fork but just run else if. This is why inside pipes.c
* we need to implement WIFEEXITED once again.
*/
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
		ft_clear_data();
		exit(CMD_NOT_FOUND);
	}
	else
	{
		g_mini.pid_id = process;
		waitpid_n_update_exit_code(process);
		g_mini.pid_id = 0;
	}
}

void	
	run_builtin_or_execve(char *cmd_w_path, char **args, int fd_in, int fd_out)
{
	g_mini.has_error = 0;
	if (run_builtin(args, fd_out))
		run_execve(cmd_w_path, args, fd_in, fd_out);
}
