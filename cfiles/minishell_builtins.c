/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_builtins.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosters <bcosters@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/10 15:14:50 by bcosters          #+#    #+#             */
/*   Updated: 2021/10/18 15:52:24 by psleziak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../extras/hfiles/minishell.h"

/*
 * we must cover just "cd" withouth notihing, it goes to root
 * we must cover echoo, or any other wrong command so it displays "command not found:echoo" and exit code is 127
 *
*/
void	ft_cd(void) // we must cover just "cd" without nothing to go to root.
{
	char	buf[4096];
	
	if (g_mini.argv[1][0] == '~')
	{
		if (g_mini.argv[1][1] == '\0')
			chdir(ft_lstfind_content(&g_mini.env, "HOME"));
		else
		{
			chdir(ft_lstfind_content(&g_mini.env, "HOME"));
			g_mini.argv[1][0] = ' ';
			if (g_mini.argv[1][1] == '/')
				g_mini.argv[1][1] = ' ';
			ft_cd();	
		}
	}
	else if(!ft_strncmp(g_mini.argv[0], "cd", 3)
			&& g_mini.argv[1] && !ft_strncmp(g_mini.argv[1], "", 2)) 
		chdir(ft_lstfind_content(&g_mini.env, "HOME"));
	else
	{
		if (chdir(g_mini.argv[1]) == -1)
			g_mini.exit_code = ft_error_handler("wrong chdir");
	}
	getcwd(buf, 100);
	ft_lstfind_update(&g_mini.env, buf, "PWD");
}

void	ft_export(void) // 2 razy ta sama zmienna update zmienna 
{
	t_list	*temp;
	char	**export_split;
	temp = g_mini.env;
	
	export_split = ft_split(g_mini.argv[1], '=');
	if (ft_lstfind_update(&g_mini.env, export_split[1], export_split[0]) == 1)
		return ;
	ft_lstadd_back(&temp, ft_lstnew(ft_split(g_mini.argv[1], '=')));
}

void	ft_unset(void)
{
	t_list	*temp;
	t_list	*previous;
	int		i;
	int		counter;

	temp = g_mini.env;
	previous = g_mini.env;
	i = 0;
	counter = 0;
	while (g_mini.argv[1][i])
		i++;
	while (temp)
	{
		if (!ft_strncmp(temp->keyword, g_mini.argv[1], i) && counter == 0)
		{
				g_mini.env = temp->next;
				break ;
		}
		if (temp->next)
			temp = temp->next;
		else
			break;
		if (!ft_strncmp(temp->keyword, g_mini.argv[1], i)) 
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

void	ft_env(void)
{
	t_list	*temp;

	temp = g_mini.env;
	while (temp)
	{
		printf("%s%s\n", ft_strjoin(temp->keyword, "="), temp->content);
		temp = temp->next;
	}
}

void	ft_exit(void)
{
	ft_clear_data(B);
	exit(EXIT_SUCCESS);
}

void	ft_pwd(void)
{
	t_list	*temp;

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

void	ft_echo(void)
{
	int i;

	i = 0;
	while (g_mini.argv[++i])
	{
		if (!ft_strncmp(g_mini.argv[i], "$?", 2))
		{
			printf("%d ", g_mini.exit_code);
			
			continue ;
		}
		printf("%s", g_mini.argv[i]);
		if (g_mini.argv[i + 1])
			printf(" ");
	}
	printf("\n");
}

void	ft_echon(void)
{
	int	i;

	i = 1;
	while (g_mini.argv[++i])
	{
		if (!ft_strncmp(g_mini.argv[i], "$?", 2))
		{
			printf("%d", g_mini.exit_code);
			continue ;
		}
		printf("%s", g_mini.argv[i]);
		if (g_mini.argv[i + 1])
			printf(" ");
	}
	printf("%%\n");
}

/*
 *	
*/

void	ft_path(void)
{
	int		i;
	int		child_id;
	char	*strjoin;
	char	*strjoin2;

	i = -1;
	child_id = fork();
	if (child_id == -1)
		printf("Error FORK\n");
	else if (child_id == 0)
	{
		while (g_mini.path[++i])
		{
			strjoin = ft_strjoin(g_mini.path[i], "/");
			strjoin2 = ft_strjoin(strjoin, g_mini.argv[0]);
			if (execve(strjoin2, &g_mini.argv[0], NULL))
				;
			free(strjoin); // this wont be freed if because execve kills the process
			free(strjoin2);
		}
		ft_error_exit("error execve");
	}
	else
	{
		wait(NULL);
		/*if (strjoin)
			free(strjoin);
		if (strjoin2)
			free(strjoin2);*/
	}
}
