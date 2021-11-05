/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tony <tony@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/02 00:35:06 by psleziak          #+#    #+#             */
/*   Updated: 2021/11/05 01:04:35 by tony             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_nr_of_commands(void)
{
	int			i;
	t_arguments	*temp;

	i = 0;
	temp = g_mini.argv;
	while (temp)
	{
		i++;
		temp = temp->next;
	}
	return (i);
}

void	run_one_command(void)
{
	int	fork_id;

	fork_id = fork();
	if (fork_id == -1)
		ft_error_handler("fork error");
	else if (fork_id == 0)
		execve(g_mini.argv->full_arg_path, g_mini.argv->args, NULL);
	else
		wait(NULL);
}

void	run_all_commands(int nr_of_commands)
{
	int			i;
	pid_t		fork_id;
	int			pipe_fd[nr_of_commands - 2][2];
	t_arguments *temp;

	temp = g_mini.argv;
	i = -1;
	while (temp)
	{
		++i;
		pipe(pipe_fd[i]);
		fork_id = fork();
		if (fork_id == -1)
			ft_error_handler("fork error");
		else if (fork_id == 0)
		{
			if (i == 0) // output idzie do pipe
			{
				printf("1st?\n");
				dup2(pipe_fd[i][1], STDOUT_FILENO);
				close(pipe_fd[i][1]);
				close(pipe_fd[i][0]);
			}
			else if (nr_of_commands - 1 == 0) // input z pipe
			{
				printf("last?\n");
				dup2(pipe_fd[i][0], STDIN_FILENO);
				close(pipe_fd[i][1]);
				close(pipe_fd[i][0]);
			}
			else
			{
				dup2(pipe_fd[i][0], STDIN_FILENO); // stdin reads from pipe
				dup2(pipe_fd[i][1], STDOUT_FILENO); // stdout writes to pipe
				close(pipe_fd[i][0]);
				close(pipe_fd[i][1]);
			}
			printf("arg path: %s\n", temp->full_arg_path);
			if (execve(temp->full_arg_path, temp->args, NULL) == -1)
				ft_error_handler("execve error");
			exit(1);
		}
		else
		{
			waitpid(fork_id, NULL, 0);
			printf("nr commands: %d, i = %d\n", nr_of_commands, i);
			close(pipe_fd[i][0]);
			//close(pipe_fd[i][1]);
			temp = temp->next;
			nr_of_commands--;
		}
		printf("stop?\n");
	}
}

void	run_commands2(void)
{
	int	nr_of_commands;

	nr_of_commands = ft_nr_of_commands();
	if (nr_of_commands == 1)
		run_one_command();
	else
		run_all_commands(nr_of_commands);
}