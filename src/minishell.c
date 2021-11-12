/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psleziak <psleziak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/10 14:56:12 by bcosters          #+#    #+#             */
/*   Updated: 2021/11/12 22:17:41 by psleziak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_handler(int sig)
{
	if (sig == SIGINT)
	{
		g_mini.exit_code = 1;
		ft_putchar_fd('\n', 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		if (!g_mini.pid_id)
			rl_redisplay();
		else
			g_mini.exit_code = 130;
	}
	if (sig == SIGQUIT)
	{
		if (g_mini.pid_id)
		{
			printf("Quit: 3\n");
			g_mini.exit_code = 131;
			rl_redisplay();
		}
		else
			printf("%s%s", g_mini.prompt, rl_line_buffer);
	}
	g_mini.has_error = 1;
}

/*
**	Memory leak protected version of gnl with readline
	-> Clears previously allocated memory for 'line' before reading again
	-> Reads from user with the given prompt
	-> IF it's a valid line
		-> Add it to the line history (ignoring empty lines)
	-> RETURN the read line;
*/
char	*rl_gnl(void)
{
	static char	*line_trimmed;
	char		*line;
	int			exit_code;

	if (line_trimmed)
		ft_strdel(&line_trimmed);
	line = readline(g_mini.prompt);
	if (!line)
	{
		exit_code = 0;
		if (g_mini.has_error)
			exit_code = g_mini.exit_code;
		ft_clear_data();
		if (exit_code)
			exit(exit_code);
		else
			exit(EXIT_SUCCESS);
	}
	// if (!*line)
	// 	exit(ft_clear_data(g_mini, B));
	if (line != NULL && line[0] != 0)
		add_history(line);
	line_trimmed = ft_strtrim(line, " \t\v");
	ft_strdel(&line);
	return (line_trimmed);
}

//! REMOVE
// void	print_args(void)
// {
// 	int			i;
// 	t_arguments	*temp = g_mini.argv;

// 	while (temp)
// 	{
// 		printf("\n\n");
// 		printf("%p\n", temp);
// 		printf("pipe: %s\n", temp->pipe_type);
// 		i = 0;
// 		while (temp->args[i])
// 		{
// 			printf("arg %i: %s\n", i, temp->args[i]);
// 			i++;
// 		}
// 		temp = temp->next;
// 	}
// }

// static t_arguments	*get_last_arg(t_arguments *lst)
// {
// 	t_arguments	*previous;
// 	t_arguments	*current;

// 	previous = lst;
// 	current = lst;
// 	if (lst == NULL)
// 		return (lst);
// 	while (current != NULL)
// 	{
// 		if (current->next == NULL)
// 			return (current);
// 		previous = current;
// 		current = current->next;
// 	}
// 	return (previous);
// }

int	main(int argc, char **argv, char **env)
{
	ft_init(argv, env);
	signal(SIGINT, ft_handler);
	signal(SIGQUIT, ft_handler);
	while (argc)
	{
		g_mini.input = rl_gnl();
		if (ft_strncmp(g_mini.input, "", 1) == 0)
			continue ;
		g_mini.argv = split_commands(g_mini.argv);
		check_commands();
		if (!g_mini.argv)
			continue ;
		run_pipe_or_single_cmd();
		if (!g_mini.has_error)
			g_mini.exit_code = 0;
	}
	return (EXIT_SUCCESS);
}
