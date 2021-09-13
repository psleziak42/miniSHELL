/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_builtins.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosters <bcosters@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/10 15:14:50 by bcosters          #+#    #+#             */
/*   Updated: 2021/09/13 16:58:11 by bcosters         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../extras/hfiles/minishell.h"

/*void	ft_env(t_minishell *mini)
{
	int x;

	x = -1;

	while (mini->env[++x] != NULL)
		;
	while (mini->env[++x] != NULL)
		ft_putstr_fd(mini->env[x], 1);
}*/

/*void	ft_exit(t_minishell *mini)
{
	free all functions
	free(mini);
	kill(mini->pid, 2);
}

void	ft_cd(t_minishell *mini)
{
	chdir(mini->argv[1]);
}

void	ft_pwd(t_minishell *mini)
{
	int x;

	x = -1;
	while (mini->env[++x] != NULL)
		;
	while (mini->env[++x])
	{
		if (!ft_strncmp(mini->env[x], "PWD=", 4))
		{
			printf("%s\n", mini->env[x] + 5);
			break;
		}
	}
	//ft_putstr_fd(getenv("PWD"), 1);
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
*/
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

	// printf("%s\n", mini->argv[0]);
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
