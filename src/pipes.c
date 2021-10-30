/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psleziak <psleziak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/29 15:58:14 by psleziak          #+#    #+#             */
/*   Updated: 2021/10/30 19:08:02 by psleziak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*static t_arguments	*get_last_valid_command()
{
	t_arguments *temp;
	t_arguments *last_valid_cmd;

	last_valid_cmd = g_mini.argv; // last valid command is the first one
	temp = g_mini.argv;
	while (temp)
	{
		if (!temp->is_valid)
			j = i;
			last_valid_cmd = temp->next;
		temp = temp->next;
		i++;
	}
	return (last_valid_cmd);
}

static t_arguments	*find_heredoc(t_arguments *temp)
{
	int	pipe_type_i;

	while (temp)
	{
		pipe_type_i = 0;
		if (temp->pipe_type[pipe_type_i] == '|')
			pipe_type_i++;
		if (temp->pipe_type[pipe_type_i] == '<' && temp->pipe_type[pipe_type_i + 1] == '<')
			return (temp);
		temp = temp->next;
	}
	return (NULL);
}

void	ft_heredoc(char *end_of_file);
{
	char	buffer[4096];
	size_t	bytes_read;

	while (bytes_read = read(0, buffer, BUFFER_SIZE)
	{
		if (ft_strncmp(buffer, end_of_file, ft_strlen(end_of_file)))
			break ;
	}
}

void	run_commands(void)
{
	t_arguments *last_valid_cmd;
	t_arguments *heredoc;

	last_valid_cmd = get_last_valid_command();
	heredoc = find_heredoc(g_mini.argv);
	while (heredoc)
	{
		ft_heredoc(heredoc->args[0]);
		heredoc = find_heredoc(heredoc->next);
	}
}*/

int	count_nr_of_commands(void)
{
	t_arguments	*temp;
	int	nr_of_commands;

	nr_of_commands = 0;
	temp = g_mini.argv;
	while (temp)
	{
		nr_of_commands++;
		temp = temp->next;
	}
	return (nr_of_commands);
}

void	run_single_command()
{
	pid_t	process;

	process = fork();
	if (process == -1)
		ft_error_handler("fork error");
	else if (process == 0)
	{
		execve(g_mini.argv->full_arg_path, g_mini.argv->args, NULL);
		ft_error_handler("command not found");
	}
	else
		wait(NULL);
}

void	run_pipes(int nr_of_commands)
{
	t_arguments	*temp;
	pid_t		process;
	int			pipe_fd[2];
	int			fd_in;
	int			fd_out;

	temp = g_mini.argv;
	dup2(101, STDOUT_FILENO);
	fd_in = STDIN_FILENO; // read part -> read FROM
	fd_out = -1;
	while (nr_of_commands-- > 0)
	{
		if (fd_out != -1)
			close(fd_out);
		if (nr_of_commands == 0)
			fd_out = STDOUT_FILENO; // write part -> write TO
		else
		{
			pipe(pipe_fd);
			fd_out = pipe_fd[1];
		}
		process = fork();
		if (process == -1)
			ft_error_handler("fork error");
		else if (process == 0)
		{
			dup2(fd_in, STDIN_FILENO);
			dup2(fd_out, STDOUT_FILENO);
			if (nr_of_commands != 0)
			{
				close(pipe_fd[0]);
				close(pipe_fd[1]);
			}
			if (temp->is_valid)
			{
				execve(temp->full_arg_path, temp->args, NULL); //TODO execve or builtins
				ft_error_handler("execve error");
				exit(EXIT_FAILURE);
			}
			exit(EXIT_SUCCESS);
		}
		else
		{
			wait(NULL);
			temp = temp->next;
			if (fd_in != STDIN_FILENO) // 1st time fd_in = stinput, and next time it will b always = ft[0];
				close(fd_in);
			if (nr_of_commands != 0)
				fd_in = pipe_fd[0];
		}
	}
}

void	run_commands(void)
{
	int		nr_of_commands;
	
	nr_of_commands = count_nr_of_commands();
	if (nr_of_commands)
		run_pipes(nr_of_commands);
	else
		run_single_command();
}