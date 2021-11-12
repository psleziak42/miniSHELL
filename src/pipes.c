/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psleziak <psleziak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/11 14:26:15 by psleziak          #+#    #+#             */
/*   Updated: 2021/11/12 17:05:13 by psleziak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	read_n_write_to_or_from_file(int fd_in, int fd_out)
{
	char	buf;

	while (read(fd_in, &buf, 1) > 0)
		write(fd_out, &buf, 1);
	exit(EXIT_SUCCESS);
}

static void	prepare_pipe_to_run(t_arguments *temp, int *fd_in, int *fd_out)
{
	if (is_redirection(temp->pipe_type))
	{
		if (get_type_of_pipe(temp->pipe_type) <= 2)
		{
			if (*fd_in != STDIN_FILENO)
				close(*fd_in);
		}
		io_table_manipulation(fd_in, fd_out, temp,
			get_type_of_pipe(temp->pipe_type));
	}
}

static void	run_pipe_child_process(t_arguments *temp, int fd_in, int fd_out)
{
	if (temp->is_valid)
	{
		if (is_redirection(temp->pipe_type))
		{
			if (!temp->args[1])
			{
				if (get_type_of_pipe(temp->pipe_type) > 2)
					read_n_write_to_or_from_file(fd_in, fd_out);
			}
			else
				run_builtin_or_execve(temp->cmd_w_path, &temp->args[1],
					fd_in, fd_out);
		}
		else
			run_builtin_or_execve(temp->cmd_w_path, temp->args, fd_in, fd_out);
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}

static void	run_pipe_parent_process(int nr_of_commands, int pipe_fd_in,
				int *fd_in, int *fd_out)
{
	if (*fd_in != STDIN_FILENO)
		close(*fd_in);
	if (*fd_out != STDOUT_FILENO)
		close(*fd_out);
	if (nr_of_commands != 0)
		*fd_in = pipe_fd_in;
}

void	run_pipes(t_arguments *temp, int nr_of_commands)
{
	pid_t		process;
	int			pipe_fd[2];
	int			fd_in;
	int			fd_out;

	fd_in = STDIN_FILENO;
	fd_out = -1;
	while (nr_of_commands-- > 0)
	{
		if (nr_of_commands == 0)
			fd_out = STDOUT_FILENO;
		if (nr_of_commands != 0)
			pipe(pipe_fd);
		if (nr_of_commands != 0)
			fd_out = pipe_fd[1];
		prepare_pipe_to_run(temp, &fd_in, &fd_out);
		process = fork();
		if (process == -1)
			error_exit("Fork error", strerror(10), GENERAL_ERR);
		else if (process == 0)
			run_pipe_child_process(temp, fd_in, fd_out);
		wait(NULL);
		run_pipe_parent_process(nr_of_commands, pipe_fd[0], &fd_in, &fd_out);
		temp = temp->next;
	}
}
