/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_and_single_cmd_utils_2.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psleziak <psleziak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/12 20:58:48 by psleziak          #+#    #+#             */
/*   Updated: 2021/11/12 20:59:15 by psleziak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	portugal_tribute(int nr_of_commands, int *fd_out, int *pipe_fd)
{
	if (nr_of_commands == 0)
		*fd_out = STDOUT_FILENO;
	if (nr_of_commands != 0)
		pipe(pipe_fd);
	if (nr_of_commands != 0)
		*fd_out = pipe_fd[1];
}
