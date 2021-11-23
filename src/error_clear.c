/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_clear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psleziak <psleziak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/26 16:33:54 by tosilva           #+#    #+#             */
/*   Updated: 2021/11/12 17:35:35 by psleziak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Clear all data from the struct
	1.	Clear the env list
	2.	Clear the path ** if it's != NULL && B is specified
	3.	Clear the argv ** if it's != NULL && A OR B is specified
	4.	Free && NULL the prompt string since it has allocated memory from init
*/
int	ft_clear_data(void)
{
	int	i;

	ft_lstclear(&g_mini.env, free);
	i = -1;
	if (g_mini.path)
	{
		while (g_mini.path[++i])
			ft_strdel(&g_mini.path[i]);
		free(g_mini.path);
		g_mini.path = NULL;
	}
	i = -1;
	if (g_mini.argv)
		ft_free_args(g_mini.argv);
	ft_strdel(&g_mini.prompt);
	return (0);
}

/*
 * ft_error_handler handles errno 
*/
void	error_exit(char *message, char *description, int exit_code)
{
	ft_clear_data();
	ft_putstr_fd(message, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putendl_fd(description, STDERR_FILENO);
	exit(exit_code);
}

void
	cmd_error_handler(char *cmd, char *arg, char *description, int exit_code)
{
	g_mini.has_error = 1;
	g_mini.exit_code = exit_code;
	ft_putstr_fd(cmd, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	if (arg)
	{
		ft_putstr_fd(arg, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	ft_putendl_fd(description, STDERR_FILENO);
}

void
	deflt_err_handler(char *message, char *description, int exit_code)
{
	g_mini.has_error = 1;
	g_mini.exit_code = exit_code;
	ft_putstr_fd(message, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putendl_fd(description, STDERR_FILENO);
}
