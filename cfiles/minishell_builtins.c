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
	int i;

	i = 1;
	while (mini->argv[++i])
	{
		printf("%s", mini->argv[i]);
		if (mini->argv[i + 1])
			printf(" ");
	}
	printf("%%\n");
}

void	ft_PATH(t_minishell *mini)
{
	int	i;
	int	child_id;
	char *strjoin;
	char *strjoin2;

	printf("%s\n", mini->argv[0]);
	child_id = fork();
	i = -1;
	if (child_id == -1)
		printf("Error FORK\n");
	else if (child_id == 0)
	{
		while(mini->PATH[++i])
		{
			strjoin = ft_strjoin(mini->PATH[i], "/");	
			strjoin2 = ft_strjoin(strjoin, mini->argv[0]);
			if(execve(strjoin2, &mini->argv[0], NULL))
				;
		}
	}
	else
		wait(NULL);
}
