/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosters <bcosters@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/10 14:56:12 by bcosters          #+#    #+#             */
/*   Updated: 2021/09/14 15:58:04 by bcosters         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../extras/hfiles/minishell.h"

//hello przemek

/*
**	Signal handler
	-> Ctrl-C == SIGINT
	-> Ctrl-\ == SIGQUIT
	-> Ctrl-D == EOF aka close inputstream or shell
*/

void	handler(int sig)
{
	if (sig == SIGINT)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	if (sig == SIGQUIT)
	{
		// printf("RECEIVED SIGQUIT\n");
		// rl_on_new_line();
		// rl_replace_line("^\\", 0);
		rl_redisplay();
	}
	return ;
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
		ft_path(mini);
}

//OBSOLETE function since getenv("PATH") does the trick

// char	**ft_get_path(/*t_minishell *mini*/)
// {
// 	// while (!(ft_strncmp(mini->env->content, "PATH=", 5)))
// 	// 	mini->env = mini->env->next;
// 	// return (ft_split(mini->env->value, ':'));
// 	return (ft_split(getenv("PATH"), ':'));
// }

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
		//ft exit
		if (!new)
			return (NULL);
		if (!head)
			head = new;
		ft_lstadd_back(&mini->env, new);
	}
	return (head);
}

void	ft_init(t_minishell *mini, char **env)
{
	mini->input = NULL;
	mini->argv = NULL;
	mini->env = ft_env_list(env, mini);
	mini->path = ft_split(getenv("PATH"), ':');
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

	if (line)
	{
		free(line);
		line = NULL;
	}
	line = readline(prompt);
	//exit + free memory
	// printf("%s, %p\n", line, line);
	if (!line)
		exit(EXIT_SUCCESS);
	if (!*line)
		exit(EXIT_SUCCESS);
	if (line != NULL && line[0] != 0)
		add_history(line);
	return (line);
}

/*
**	The 3rd parameter of main IS the environment always

*/

int	main(int argc, char **argv, char **env)
{
	t_minishell	mini;
	char		*prompt;

	ft_init(&mini, env);
	signal(SIGINT, handler);
	signal(SIGQUIT, handler);
	while (argc)
	{
		//some fun way to make the prompt
		prompt = ft_strtrim(argv[0], "./");
		prompt = ft_strjoin(prompt, "42: ");
		mini.input = rl_gnl(prompt);
		// add_history(mini->input);
		//the escape chars + single/double quotes need to be handled
		mini.argv = ft_split(mini.input, 32);
		functions(&mini);
	}
	free(prompt);
	// free mini->argv in a while loop
	//no memory getting cleared yet
}
