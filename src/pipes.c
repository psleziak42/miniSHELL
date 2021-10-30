/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psleziak <psleziak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/29 15:58:14 by psleziak          #+#    #+#             */
/*   Updated: 2021/10/30 15:29:41 by psleziak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_arguments	*get_last_valid_command()
{
	t_arguments *temp;
	t_arguments *last_valid_cmd;

	last_valid_cmd = g_mini.argv; // last valid command is the first one
	temp = g_mini.argv;
	while (temp)
	{
		if (!temp->is_valid)
			last_valid_cmd = temp->next;
		temp = temp->next;
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

void	ft_heredoc(char *end_of_file)
{
	end_of_file = (char *)end_of_file;
	/*char	buffer[4096];
	size_t	bytes_read;

	while ((bytes_read = read(0, buffer, BUFFER_SIZE)))
	{
		if (ft_strncmp(buffer, end_of_file, ft_strlen(end_of_file)))
			break ;
	}*/
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
}
