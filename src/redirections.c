/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psleziak <psleziak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/11 13:41:51 by psleziak          #+#    #+#             */
/*   Updated: 2021/11/11 18:39:06 by psleziak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_input(int *in, t_arguments *temp)
{
	int	file_fd;

	file_fd = open(temp->args[0], O_RDONLY);
	if (file_fd == -1)
		deflt_err_handler(temp->args[0], strerror(2), GENERAL_ERR);
	*in = file_fd;
}

/*
*	We dont free line because readline frees it automatically (?)
*/
static void	run_heredoc(char *end_of_file, int file_fd)
{
	char	*line;
	char	*new_line;
	size_t	eof_length;

	eof_length = ft_strlen(end_of_file);
	while (1)
	{
		line = readline("heredoc> ");
		if (!ft_strncmp(end_of_file, line, eof_length + 1))
			break ;
		new_line = ft_strjoin(line, "\n");
		if (!new_line)
			return ;
		write(file_fd, new_line, ft_strlen(new_line));
		free(new_line);
	}
}

/*
*	we open file to write to it only
*	then we open it again to read from it only
*/
void	ft_input_heredoc(int *in, t_arguments *temp)
{
	int		file_fd;
	char	*file_dir;

	file_dir = ft_strjoin(getenv("HOME"), "/heredoc.txt");
	if (temp->pipe_type[0] == '|')
		file_fd = open(file_dir, O_WRONLY | O_APPEND | O_CREAT, 0666);
	else
		file_fd = open(file_dir, O_WRONLY | O_TRUNC | O_CREAT, 0666);
	run_heredoc(temp->args[0], file_fd);
	close(file_fd);
	file_fd = open(file_dir, O_RDONLY);
	if (file_fd == -1)
		deflt_err_handler(file_dir, strerror(77), GENERAL_ERR);
	free(file_dir);
	*in = file_fd;
}

void	ft_output(int *out, t_arguments *temp)
{
	int		file_fd;

	file_fd = open(temp->args[0], O_WRONLY | O_TRUNC | O_CREAT, 0666);
	if (file_fd == -1)
		deflt_err_handler(temp->args[0], strerror(2), GENERAL_ERR);
	if (temp->next && get_type_of_pipe(temp->next->pipe_type) >= 3)
		close(file_fd);
	else
		*out = file_fd;
}

void	ft_ouptut_append(int *out, t_arguments *temp)
{
	int	file_fd;

	file_fd = open(temp->args[0], O_WRONLY | O_APPEND | O_CREAT, 0666);
	if (file_fd == -1)
		deflt_err_handler(temp->args[0], strerror(2), GENERAL_ERR);
	if (temp->next && get_type_of_pipe(temp->next->pipe_type) >= 3)
		close(file_fd);
	else
		*out = file_fd;
}
