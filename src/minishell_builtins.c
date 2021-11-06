/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_builtins.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosilva <tosilva@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/10 15:14:50 by bcosters          #+#    #+#             */
/*   Updated: 2021/11/06 22:49:26 by tosilva          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * we must cover just "cd" withouth notihing, it goes to root
 * we must cover echoo, or any other wrong command so it displays "command not found:echoo" and exit code is 127
 *
*/
// TODO: join ~/Doc with full path
void	ft_cd(char **args) // we must cover just "cd" without nothing to go to root.
{
	char	buf[4096];
	
	if (!ft_strncmp(args[0], "cd", 3) && !args[1]) 
		chdir(ft_lstfind_content(&g_mini.env, "HOME"));
	else if (args[1][0] == '~')
	{
		chdir(ft_lstfind_content(&g_mini.env, "HOME"));
		if (args[1][1] != '\0')
		{
			ft_memmove(&args[1][0], &args[1][2], ft_strlen(args[1]) - 1);
			chdir(args[1]);
		}
	}
	else
	{
		if (chdir(args[1]) == -1)
			g_mini.exit_code = ft_error_handler("wrong chdir");
	}
	getcwd(buf, 4096);
	ft_lstfind_update(&g_mini.env, buf, "PWD");
}

void	ft_export(char **args) //* Fully working
{
	t_list	*temp;
	char	**export_split;
	temp = g_mini.env;
	
	if (!ft_strchr(args[1], '='))
		return ;
	export_split = ft_split(args[1], '=');
	if (!export_split[1])
		export_split[1] = ft_strdup("");
	if (ft_lstfind_update(&g_mini.env, export_split[1], export_split[0]) == 1)
	{
		free(export_split[0]);
		free(export_split[1]);
		return ;
	}
	ft_lstadd_back(&temp, ft_lstnew(export_split));
}

void	ft_unset(char **args)
{
	t_list	*temp;
	t_list	*previous;
	int		i;
	int		counter;

	temp = g_mini.env;
	previous = g_mini.env;
	i = 0;
	counter = 0;
	while (args[1][i])
		i++;
	while (temp)
	{
		if (!ft_strncmp(temp->keyword, args[1], i) && counter == 0)
		{
				g_mini.env = temp->next;
				break ;
		}
		if (temp->next)
			temp = temp->next;
		else
			break;
		if (!ft_strncmp(temp->keyword, args[1], i)) 
		{
			previous->next = temp->next;
			free(temp);
			break ;
		}
		if (counter != 0)
			previous = previous->next;
		counter++;
	}
}

void	ft_env(char **args)
{
	t_list	*temp;

	args = (char **)args;
	temp = g_mini.env;
	while (temp)
	{
		printf("%s%s\n", ft_strjoin(temp->keyword, "="), temp->content);
		temp = temp->next;
	}
}

void	ft_exit(char **args)
{
	args = (char **)args;
	printf("%s\n", "exit");
	ft_clear_data();
	exit(EXIT_SUCCESS);
}

void	ft_pwd(char **args)
{
	t_list	*temp;

	args = (char **)args;
	temp = g_mini.env;
	while (temp)
	{
		if (!ft_strncmp(temp->keyword, "PWD", 3))
		{	
			ft_putstr_fd(temp->content, 1);
			break;
		}
		temp = temp->next;
	}
}

void	ft_echo(char **args)
{
	int i;

	i = 0;
	if (args[1] && !ft_strncmp(args[1], "-n", 3))
		i = 1;
	while (args[++i])
	{
		if (!ft_strncmp(args[i], "$?", 2))
			printf("%d", g_mini.exit_code);
		else
			printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
	}
	if (!args[1] || (args[1] && ft_strncmp(args[1], "-n", 3) != 0))
		printf("\n");
}

/*
 *	
*/

// void	ft_path(char **args)
// {
// 	int		i;
// 	int		child_id;
// 	char	*strjoin;
// 	char	*strjoin2;

// 	i = -1;
// 	child_id = fork();
// 	if (child_id == -1)
// 		printf("Error FORK\n");
// 	else if (child_id == 0)
// 	{
// 		while (g_mini.path[++i])
// 		{
// 			strjoin = ft_strjoin(g_mini.path[i], "/");
// 			strjoin2 = ft_strjoin(strjoin, args[0]);
// 			if (execve(strjoin2, &args[0], NULL))
// 				;
// 			free(strjoin); // this wont be freed if because execve kills the process
// 			free(strjoin2);
// 		}
// 		ft_error_exit("error execve");
// 	}
// 	else
// 	{
// 		wait(NULL);
// 		/*if (strjoin)
// 			free(strjoin);
// 		if (strjoin2)
// 			free(strjoin2);*/
// 	}
// }
