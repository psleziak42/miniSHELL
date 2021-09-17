/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_builtins.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosters <bcosters@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/10 15:14:50 by bcosters          #+#    #+#             */
/*   Updated: 2021/09/17 00:06:50 by psleziak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../extras/hfiles/minishell.h"

void	ft_export(void) // 2 razy ta sama zmienna update zmienna 
{
	t_list	*temp;

	temp = g_mini.env;
	ft_lstadd_back(&temp, ft_lstnew(ft_split(g_mini.argv[1], '=')));
}

/*
 *	This shit i still didnt fix sorry Ben
*/

void	ft_unset(void) // co za kurwa gowno
{
	t_list	*temp;
	t_list	*current;
	int		i;

	temp = g_mini.env;
	current = g_mini.env;
	i = 0;
	while (g_mini.argv[1])
		i++;
	while (temp)
	{
		if (!ft_strncmp(temp->next->keyword, g_mini.argv[1], i))
		{
			current = current->next;
			temp->next = current->next;
			free(current);
			current = NULL;
			break ;
		}
		temp = temp->next;
		current = current->next;
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

/*
	pwd is blank when i do cd ~, but PWD works...
	cd .. is adding .. to curent dir
	cd <folder> works correctly
	Im gonna work on it tomorrow Ben
	I think cd needs separate file
*/

void	ft_pwd(void)
{
	t_list	*temp;

	temp = g_mini.env;
	while (temp)
	{
		if (!ft_strncmp(temp->keyword, "PWD", 3))
		{	
			printf("inside pwd: %s\n", temp->content);
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
		printf("%s", g_mini.argv[i]);
		if (g_mini.argv[i + 1])
			printf(" ");
	}
	printf("%%\n");
}

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
			free(strjoin);
			free(strjoin2);
		}
	}
	else
		wait(NULL);
}
