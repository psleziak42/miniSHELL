/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_and_single_cmd_utils.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psleziak <psleziak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/11 14:34:47 by tosilva           #+#    #+#             */
/*   Updated: 2021/11/12 20:59:49 by psleziak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_redirection(char *pipe_type)
{
	int	i;

	i = 0;
	if (pipe_type[i] == '|')
		i++;
	if (pipe_type[i] && pipe_type[i] != '|')
		return (TRUE);
	return (FALSE);
}

int	get_type_of_pipe(char *pipe_type)
{
	int	i;

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

void	
	io_table_manipulation(int *in, int *out, t_arguments *temp, int pipe_type)
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

void	waitpid_n_update_exit_code(pid_t process)
{
	int	status;

	waitpid(process, &status, 0);
	if (WIFEXITED(status))
	{
		g_mini.has_error = 1;
		g_mini.exit_code = WEXITSTATUS(status);
	}
}

void	read_n_write_to_or_from_file(int fd_in, int fd_out, bool to_exit)
{
	char	buf;

	while (read(fd_in, &buf, 1) > 0)
		write(fd_out, &buf, 1);
	if (to_exit)
		exit(g_mini.exit_code);
}
