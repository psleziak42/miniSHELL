/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psleziak <psleziak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/02 00:35:06 by psleziak          #+#    #+#             */
/*   Updated: 2021/11/10 16:47:28 by psleziak         ###   ########.fr       */
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
static void	run_builtin_or_execve(char *full_arg_path, char **args, int fd_in, int fd_out)
{
	int i;
	int	var_len;
	int	process;

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
				return ;
		}
	}
	process = 0;
	if (g_mini.argv->next == NULL) // pipe wont do the fork it goes to execve directly.
		process = fork();
	if (process == -1)
		ft_default_error_handler("Fork error", strerror(10));
	else if (process == 0)
	{
		dup2(fd_in, STDIN_FILENO);
		dup2(fd_out, STDOUT_FILENO);
		if (fd_in != STDIN_FILENO)
			close(fd_in);
		if (fd_out != STDOUT_FILENO)
			close(fd_out);
		execve(full_arg_path, args, NULL);
		exit(EXIT_FAILURE);
	}
	else
	{
		g_mini.pid_id = process;
		wait(NULL);
		g_mini.pid_id = 0;
	}
	// ft_cmd_error_handler(args[0], NULL, INVALID_COMMAND);
}

static int	count_nr_of_commands(void)
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

static bool	is_redirection(char *pipe_type)
{
	int i;

	i = 0;
	if (pipe_type[i] == '|')
		i++;
	if (pipe_type[i] && pipe_type[i] != '|')
		return (TRUE);
	return (FALSE);
}

int get_type_of_pipe(char *pipe_type)
{
	int i;

	i = 0;
	if (pipe_type[i] == '|')
		i++;
	if (pipe_type[i] == '<' && pipe_type[i + 1] == '\0')
		return (1);
	if (pipe_type[i] == '<' && pipe_type[i + 1] == '<')
		return (2);
	if (pipe_type[i] == '>' && pipe_type[i + 1] == '\0')
		return (3);
	if (pipe_type[i] == '>' && pipe_type[i + 1] == '>')
		return (4);
	return (0);
}

void	ft_ouptut_append(int *out, t_arguments *temp)
{
	int	file_fd;

	file_fd = open(temp->args[0], O_WRONLY | O_APPEND | O_CREAT, 0666);
	if (file_fd == -1)
		ft_default_error_handler(temp->args[0], strerror(2));
	if (temp->next && get_type_of_pipe(temp->next->pipe_type) >= 3)
		close(file_fd);
	else
		*out = file_fd;
}

void	ft_output(int *out, t_arguments *temp)
{
	int		file_fd;

	file_fd = open(temp->args[0], O_WRONLY | O_TRUNC | O_CREAT, 0666);
	if (file_fd == -1)
		ft_default_error_handler(temp->args[0], strerror(2));
	if (temp->next && get_type_of_pipe(temp->next->pipe_type) >= 3)
		close(file_fd);
	else
		*out = file_fd;
}

void	ft_input(int *in, t_arguments *temp)
{
	int	file_fd;

	file_fd = open(temp->args[0], O_RDONLY);
	if (file_fd == -1)
		ft_default_error_handler(temp->args[0], strerror(2));
	*in = file_fd;
}

/*
*	we open file to write to it only
*	then we open it again to read from it only
*/

void	ft_input_heredoc(int *in, t_arguments *temp)
{
	int		file_fd;
	char	*file_dir;
	char	*line;
	char	*new_line;
	size_t	eof_length;	

	file_dir = ft_strjoin(getenv("HOME"), "/heredoc.txt");
	if (temp->pipe_type[0] == '|')
		file_fd = open(file_dir, O_WRONLY | O_APPEND | O_CREAT, 0666);
	else
		file_fd = open(file_dir, O_WRONLY | O_TRUNC | O_CREAT, 0666);
	eof_length = ft_strlen(temp->args[0]);
	while (1)
	{
		line = readline("heredoc> ");
		if (!ft_strncmp(temp->args[0], line, eof_length + 1))
			break ;
		new_line = ft_strjoin(line, "\n");
		if (!new_line)
			return ;
		write(file_fd, new_line, ft_strlen(new_line));
		free(new_line);
	}
	close(file_fd);
	file_fd = open(file_dir, O_RDONLY);
	if (file_fd == -1)
		ft_default_error_handler(file_dir, strerror(77));
	free(file_dir);
	*in = file_fd;
}

static void io_table_manipulation(int *in, int *out, t_arguments *temp, int pipe_type)
{
	if (pipe_type == 1)
		ft_input(in, temp);
	if (pipe_type == 2)
		ft_input_heredoc(in, temp);
	if (pipe_type == 3)
		ft_output(out, temp);
	if (pipe_type == 4)
		ft_ouptut_append(out, temp);
}

static void	read_n_write_to_or_from_file(int fd_in, int fd_out)
{
	char	buf;

	while (read(fd_in, &buf, 1) > 0)
		write(fd_out, &buf, 1);
	exit(EXIT_SUCCESS);
}

static void	run_pipes(int nr_of_commands)
{
	t_arguments	*temp;
	pid_t		process;
	int			pipe_fd[2];
	int			fd_in;
	int			fd_out;

	temp = g_mini.argv;
	fd_in = STDIN_FILENO; // read part -> read FROM
	fd_out = -1;
	while (nr_of_commands-- > 0)
	{
		if (nr_of_commands == 0)
			fd_out = STDOUT_FILENO; // write part -> write TO
		else
		{
			pipe(pipe_fd);
			fd_out = pipe_fd[1];
		}
		if (is_redirection(temp->pipe_type))
		{
			if (get_type_of_pipe(temp->pipe_type) >= 3)
				close(fd_out);
			else
				close(fd_in);
			io_table_manipulation(&fd_in, &fd_out, temp, get_type_of_pipe(temp->pipe_type));
		}
		process = fork();
		if (process == -1)
			ft_default_error_handler("Fork error", strerror(10));
		else if (process == 0)
		{
			// dup2(fd_in, STDIN_FILENO);
			// dup2(fd_out, STDOUT_FILENO);
			// if (nr_of_commands != 0)
			// {
			// 	close(pipe_fd[0]);
			// 	close(pipe_fd[1]);
			// }
			if (temp->is_valid)
			{
				if (is_redirection(temp->pipe_type))
				{
					if (!temp->args[1])
					{
							// read_n_write_to_or_from_file(fd_in, fd_out); // it is now output of the pipe due to dup2
						// else
						if (get_type_of_pipe(temp->pipe_type) > 2)
						{
							read_n_write_to_or_from_file(fd_in, fd_out); // 
							// if (nr_of_commands != 0)
							// {
							// 	if (is_redirection(temp->next->pipe_type) &)
							// 	close(fd_out);
							// 	fd_out = open(temp->args[0], O_RDONLY);
							// 	read_n_write_to_or_from_file(fd_out, STDOUT_FILENO);
							// }
						}
					}
					else
						run_builtin_or_execve(temp->full_arg_path, &temp->args[1], fd_in, fd_out);
				}
				else
					run_builtin_or_execve(temp->full_arg_path, temp->args, fd_in, fd_out);
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
			if (fd_out != STDOUT_FILENO)
				close(fd_out);
			if (nr_of_commands != 0)
				fd_in = pipe_fd[0];
		}
	}
}

static void	run_single_command()
{
	int	fd_in;
	int	fd_out;
	
	fd_in = STDIN_FILENO;
	fd_out = STDOUT_FILENO;
	if (is_redirection(g_mini.argv->pipe_type))
	{
		io_table_manipulation(&fd_in, &fd_out, g_mini.argv, get_type_of_pipe(g_mini.argv->pipe_type));
		// if (get_type_of_pipe(g_mini.argv->pipe_type) <= 2)
		// {
		// 	//dup2(fd_in, STDIN_FILENO);
		// 	//read_n_write_to_or_from_file(fd_in, STDOUT_FILENO); // it is now output of the pipe due to dup2
		// }
		// else
		// {
		// 	dup2(fd_out, STDOUT_FILENO);
		// 	// read_n_write_to_or_from_file(fd_in, fd_out);
		// 	// fd_out = open(g_mini.argv->args[0], O_RDONLY);
		// 	// read_n_write_to_or_from_file(fd_out, STDOUT_FILENO);
		// }
		// if (!g_mini.argv->args[1])
		// {
		// 	if (get_type_of_pipe(g_mini.argv->pipe_type) <= 2)
		// 		close(fd_in);
		// 	else
		// 		close(fd_out);
		// 	return ;
		// } 
		if (g_mini.argv->args[1])
			run_builtin_or_execve(g_mini.argv->full_arg_path, &g_mini.argv->args[1], fd_in, fd_out);
	}
	else
		run_builtin_or_execve(g_mini.argv->full_arg_path, g_mini.argv->args, fd_in, fd_out);
	// if (get_type_of_pipe(g_mini.argv->pipe_type) <= 2)
	// 	close(fd_in);
	// else
	// 	close(fd_out);
}

void	run_commands(void)
{
	int		nr_of_commands;
	char	*path_to_unlink_heardoc;
	
	nr_of_commands = count_nr_of_commands();
	if (nr_of_commands > 1)
		run_pipes(nr_of_commands);
	else
		run_single_command();
	path_to_unlink_heardoc = ft_strjoin(getenv("HOME"), "/heredoc.txt");
	unlink(path_to_unlink_heardoc);
	ft_memdel((void **)&path_to_unlink_heardoc);
}
