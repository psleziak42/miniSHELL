/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_pipe_or_single_cmd.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psleziak <psleziak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/02 00:35:06 by psleziak          #+#    #+#             */
/*   Updated: 2021/11/12 22:20:09 by psleziak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_nr_of_commands(void)
{
	t_arguments	*temp;
	int			nr_of_commands;

	nr_of_commands = 0;
	temp = g_mini.argv;
	while (temp)
	{
		nr_of_commands++;
		temp = temp->next;
	}
	return (nr_of_commands);
}

static void	run_single_command(void)
{
	int	fd_in;
	int	fd_out;

	fd_in = STDIN_FILENO;
	fd_out = STDOUT_FILENO;
	if (is_redirection(g_mini.argv->pipe_type))
	{
		io_table_manipulation(&fd_in, &fd_out, g_mini.argv,
			get_type_of_pipe(g_mini.argv->pipe_type));
		if (g_mini.argv->args[1])
			run_builtin_or_execve(g_mini.argv->cmd_w_path,
				&g_mini.argv->args[1], fd_in, fd_out);
	}
	else
		run_builtin_or_execve(g_mini.argv->cmd_w_path,
			g_mini.argv->args, fd_in, fd_out);
}

void	run_pipe_or_single_cmd(void)
{
	int		nr_of_commands;
	char	*path_to_unlink_heardoc;

	nr_of_commands = count_nr_of_commands();
	if (nr_of_commands > 1)
		run_pipes(g_mini.argv, nr_of_commands);
	else
		run_single_command();
	path_to_unlink_heardoc = ft_strjoin(getenv("HOME"), "/heredoc.txt");
	unlink(path_to_unlink_heardoc);
	ft_memdel((void **)&path_to_unlink_heardoc);
}
