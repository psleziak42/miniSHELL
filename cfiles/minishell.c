/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosters <bcosters@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/10 14:56:12 by bcosters          #+#    #+#             */
/*   Updated: 2021/09/14 00:15:20 by psleziak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../extras/hfiles/minishell.h"

//hello

/*
**	Signal handler
	-> Ctrl-C == SIGINT
	-> Ctrl-\ == SIGQUIT
	-> Ctrl-D == EOF aka close inputstream or shell
*/

void	ft_handler(int signal)
{
	// pid_t	pid;

	//getpid is an illegal function
	// pid = getpid();
	// printf("signal = %d\n", signal);
	if (signal == SIGINT)
	{

		printf("\n");
		rl_on_new_line();
		// rl_replace_line("", 0);
		rl_redisplay();
	}
	if (signal == SIGQUIT)
	{
		// printf("SIGQUIT RECEIVED\n");
	}
	if (signal == SIGUSR1)
		kill(0, SIGKILL);
	return ;
}

void	functions(t_minishell *mini)
{
	if (!(ft_strncmp(mini->input, "echo -n", 7)))
		ft_echon(mini);
	else if (!(ft_strncmp(mini->input, "echo", 4)))
		ft_echo(mini);
	/*else if (!(ft_strncmp(mini->input, "cd", 2)))
		ft_cd(mini);*/
	else if (!(ft_strncmp(mini->input, "pwd", 3)))
		ft_pwd(mini);
	else if (!(ft_strncmp(mini->input, "export", 6)))
		ft_export(mini);
	/*else if (!(ft_strncmp(mini->input, "unset", 5)))
		ft_unset(mini);  printf("a\n");*/
	else if (!(ft_strncmp(mini->input, "env", 3)))
		ft_env(mini);
	else if (!(ft_strncmp(mini->input, "exit", 4)))
		ft_exit(mini);
	else
		ft_path(mini);
}

//OBSOLETE function since getenv("PATH") does the trick

char	**ft_get_path(t_minishell *mini)
{
	while (mini->env)
	{
		if (!ft_strncmp(mini->env->keyword, "PATH", 4))
			return (ft_split(mini->env->content, ':'));
		mini->env = mini->env->next;
	}
	return(0); // if there is not path then return error
}

// CREATING LIST //

t_list	*ft_env_list(char **env, t_minishell *mini)
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
		if (!new)
		{
			//ft_exit
			return (NULL);
		}
		if (!head)
			head = new;
		ft_lstadd_back(&mini->env, new);
	}
	return (head);
}

t_minishell	*ft_init(char **env)
{
	int			i;
	t_minishell	*mini;

	i = -1;
	mini = malloc(sizeof(t_minishell));
	if (!mini)
		return (NULL);
	mini->env = ft_env_list(env, mini);
	mini->path = ft_get_path(mini);
	i = -1;
	return (mini);
}

/*
**	Memory leak protected version of gnl with readline
	-> Clears previously allocated memory before reading again
	-> Reads from user with the given prompt
	-> IF it's a valid line
		-> Add it to the line history (ignoring empty lines)
	-> RETURN the read line;
*/

char	*rl_gnl(char *prompt)
{
	static char	*line;

	if (line) // what is this for Ben?? //
	{
		free(line);
		line = NULL;
	}
	line = readline(prompt);
	//exit + free memory
	if (!line)
		exit(EXIT_SUCCESS);
	if (line != NULL && line[0] != 0)
		add_history(line);
	return (line);
}

int	main(int argc, char **argv, char **env)
{
	t_minishell	*mini;
	char		*prompt;

	mini = ft_init(env);
	signal(SIGINT, ft_handler);
	signal(SIGQUIT, ft_handler);
	while (argc)
	{
		//some fun way to make the prompt
		prompt = ft_strtrim(argv[0], "./");
		prompt = ft_strjoin(prompt, "42: ");
		mini->input = rl_gnl(prompt);
		ft_init(env);
		mini->argv = ft_split(mini->input, ' ');
		//the escape chars + single/double quotes need to be handled
		functions(mini);
	}
	//ft_exit
	free(prompt);
	free(mini);
	// free mini->argv in a while loop
	//no memory getting cleared yet
}
