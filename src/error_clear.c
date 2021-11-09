/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_clear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosilva <tosilva@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/26 16:33:54 by tosilva           #+#    #+#             */
/*   Updated: 2021/11/09 13:05:48 by tosilva          ###   ########.fr       */
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
void	ft_error_exit(const char *errmessage)
{
	ft_clear_data();
	perror(errmessage);
	exit(EXIT_FAILURE);
}

void	ft_cmd_error_handler(char *command, char *argument, char *description)
{
	ft_putstr_fd(command, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	if (argument)
	{
		ft_putstr_fd(argument, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	ft_putendl_fd(description, STDERR_FILENO);
}

void	ft_default_error_handler(char *message, char *description)
{
	ft_putstr_fd(message, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putendl_fd(description, STDERR_FILENO);
}
