#include "../extras/hfiles/minishell.h"

void	handler(int signal)
{
	pid_t pid;

	pid = getpid();
	if (signal == 2)
	{
		kill(pid, 9);
	}
	if (signal == 3)
	{
		kill(pid, 9);
	}
}

void	functions(t_minishell *mini)
{

	if (!(ft_strncmp(mini->input, "echo -n", 7)))
		ft_echon(mini);
	/*else if (!(ft_strncmp(mini->input, "echo", 4)))
		ft_echo(mini);
	else if (!(ft_strncmp(mini->input, "cd", 2)))
		ft_cd(mini);
	else if (!(ft_strncmp(mini->input, "pwd", 3)))
		ft_pwd(mini);
	else if (!(ft_strncmp(mini->input, "export", 6)))
		ft_export(mini); printf("a\n");
	else if (!(ft_strncmp(mini->input, "unset", 5)))
		ft_unset(mini);  printf("a\n");
	else if (!(ft_strncmp(mini->input, "env", 3)))
		ft_env(mini);
	else if (!(ft_strncmp(mini->input, "exit", 4)))
		ft_exit(mini);*/
	else	
		ft_PATH(mini);
}

char **ft_get_path(t_minishell *mini)
{
	while (!(ft_strncmp(mini->env->content, "PATH=", 5))) 
		mini->env = mini->env->next;
	return(ft_split(mini->env->value, ':'));
}

/*char	**ft_get_path(char **env)
{
	int	i;

	i = -1;
	while (env[++i])
		;
	while (ft_strncmp(env[++i], "PATH=", 5))
		;
	return (ft_split(env[i] + 5, ':'));
}*/

t_list *ft_env_list(char **env, t_minishell *mini)
{
	int		i;
	t_list	*new;
	t_list	*head;

	new = NULL;
	head = NULL;
	i = -1;
	while (env[++i])
	{
		new = ft_lstnew(ft_split(env[i], '='));
		if (!head)
			head = new;
		ft_lstadd_back(&mini->env, new);
	}
	return (head);
}

t_minishell	*ft_init(char **env)
{
	int	i;
	t_minishell *mini;

	i = -1;
	mini = malloc(sizeof(t_minishell));
	if (!mini)
		return (0);
	mini->env = ft_env_list(env, mini); 
	/*while (mini->env)
	{
		printf("%s\n", mini->env->content);
		printf("%s\n", mini->env->value);
		mini->env = mini->env->next;
		if (!mini->env)
			break;
	}*/
	//mini->env = env;
	mini->pid = getpid();
	mini->PATH = ft_get_path(mini);
	/*while (mini->PATH[++i])
		printf("%s\n", mini->PATH[i]);*/
	i = -1;
	return (mini);
}

int main(int argc, char **env)
{
	t_minishell	*mini;

	mini = ft_init(env);
	while (argc)
	{
		signal(SIGINT, handler);
		signal(SIGQUIT, handler);
		mini->input = readline("minishell42: ");
		add_history(mini->input);
		mini->argv = ft_split(mini->input, 32);
		functions(mini);
	}
}
