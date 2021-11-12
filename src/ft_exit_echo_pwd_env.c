/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit_echo_pwd_env.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psleziak <psleziak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/10 15:14:50 by bcosters          #+#    #+#             */
/*   Updated: 2021/11/12 15:46:44 by psleziak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_env(char **args, int fd_out)
{
	t_list	*temp;

	if (args[1])
	{
		cmd_error_handler(args[0], args[1], strerror(2), GENERAL_ERR);
		return ;
	}
	temp = g_mini.env;
	while (temp)
	{
		ft_putstr_fd(temp->keyword, fd_out);
		ft_putstr_fd("=", fd_out);
		ft_putendl_fd(temp->content, fd_out);
		temp = temp->next;
	}
}

/* why free_temp_args doesnt take adrres to double pointer to be freed?
*
*	Normally with ft_memdel() we use a double pointer because we want to 
*		set the pointer being freed to NULL. In this case, we could do that
*		but because we exit() right away we don't need that (setting it to NULL)
*		It is just easier to read and to write.
*/
static void	free_temp_args(char **args_to_free)
{
	ft_memdel((void **)&args_to_free[0]);
	ft_memdel((void **)&args_to_free[1]);
}

void	ft_exit(char **args, int fd_out)
{
	int		exit_code;
	char	*temp_args[2];

	if (args[1])
		temp_args[0] = ft_strdup(args[1]);
	else
		temp_args[0] = NULL;
	if (args[1] && args[2])
		temp_args[1] = ft_strdup(args[2]);
	else
		temp_args[1] = NULL;
	exit_code = g_mini.exit_code;
	ft_putendl_fd("exit", fd_out);
	if (temp_args[0] && temp_args[1] && ft_isnumber(temp_args[0]))
		cmd_error_handler("exit", NULL, TOO_MANY_ARGS, GENERAL_ERR);
	else
	{	
		ft_clear_data();
		if (temp_args[0] && ft_isnumber(temp_args[0]))
			exit_code = ft_atoi(temp_args[0]);
		else if (temp_args[0])
			cmd_error_handler("exit", temp_args[0], NUM_REQUIRED, OUT_OF_RANGE);
		free_temp_args(temp_args);
		exit(exit_code);
	}
}

void	ft_pwd(char **args, int fd_out)
{
	t_list	*temp;

	fd_out = (int)fd_out;
	args = (char **)args;
	temp = g_mini.env;
	while (temp)
	{
		if (!ft_strncmp(temp->keyword, "PWD", 4))
		{	
			ft_putendl_fd(temp->content, 1);
			break ;
		}
		temp = temp->next;
	}
}

void	ft_echo(char **args, int fd_out)
{
	int	i;

	i = 0;
	if (args[1] && !ft_strncmp(args[1], "-n", 3))
		i = 1;
	while (args[++i])
	{
		if (!ft_strncmp(args[i], "$?", 2))
			ft_putnbr_fd(g_mini.exit_code, fd_out);
		else
			ft_putstr_fd(args[i], fd_out);
		if (args[i + 1])
			ft_putstr_fd(" ", fd_out);
	}
	if (!args[1] || (args[1] && ft_strncmp(args[1], "-n", 3) != 0))
		ft_putendl_fd("", fd_out);
}
