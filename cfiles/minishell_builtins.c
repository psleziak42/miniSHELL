/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_builtins.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosters <bcosters@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/10 15:14:50 by bcosters          #+#    #+#             */
/*   Updated: 2021/09/14 15:00:29 by psleziak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../extras/hfiles/minishell.h"

void	ft_export(t_minishell *mini) // blad
{
	t_list	*temp;

	temp = mini->env;
	ft_lstadd_back(&temp, ft_lstnew(ft_split(mini->argv[1], '=')));
	free(temp);
}

void	ft_env(t_minishell *mini)
{
	t_list	*temp;

	temp = mini->env;
	while (temp)	
	{
		printf("%s%s\n", ft_strjoin(temp->keyword, "="), temp->content);
		temp = temp->next;
	}
	free(temp);
}

void	ft_exit(t_minishell *mini)
{
	//free all functions
	free(mini);
	ft_handler(SIGUSR1);
}

/*void	ft_cd(t_minishell *mini)
{
	chdir(mini->argv[1]);
}*/

void	ft_pwd(t_minishell *mini)
{
	t_list	*temp;

	temp = mini->env;
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

void	ft_echo(t_minishell *mini)
{
	int i;

	i = 0;
	while (mini->argv[++i])
	{
		printf("%s", mini->argv[i]);
		if (mini->argv[i + 1])
			printf(" ");
	}
	printf("\n");
}

void	ft_echon(t_minishell *mini)
{
	int	i;

	i = 1;
	while (mini->argv[++i])
	{
		printf("%s", mini->argv[i]);
		if (mini->argv[i + 1])
			printf(" ");
	}
	printf("%%\n");
}

void	ft_path(t_minishell *mini)
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
		while (mini->path[++i])
		{
			strjoin = ft_strjoin(mini->path[i], "/");
			strjoin2 = ft_strjoin(strjoin, mini->argv[0]);
			if (execve(strjoin2, &mini->argv[0], NULL))
				;
			free(strjoin);
			free(strjoin2);
		}
	}
	else
		wait(NULL);
}
