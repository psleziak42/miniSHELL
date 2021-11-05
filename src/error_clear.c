/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_clear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tony <tony@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/26 16:33:54 by tosilva           #+#    #+#             */
/*   Updated: 2021/11/02 00:47:07 by tony             ###   ########.fr       */
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

int	ft_error_handler(const char *errmessage)
{
	//int	save_errno;

	//save_errno = errno;
	perror(errmessage);
	return (errno);
}
