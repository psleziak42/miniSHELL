/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosters <bcosters@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/10 14:56:12 by bcosters          #+#    #+#             */
/*   Updated: 2021/09/22 15:42:45 by psleziak         ###   ########.fr       */
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

/*void	ft_handler(int sig)
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
		printf("g_minishell42: ");
		rl_redisplay();
	}
	// if (sig == SIGUSR1)
	// 	kill(0, SIGKILL);
	return ;
}*/

void	functions(void)
{
//	if (g_mini.argv[0][0] == '$')
//		ft_expand_var(); // working for argv[1];
	if (!(ft_strncmp(g_mini.input, "echo -n", 7)))
		ft_echon();
	else if (!(ft_strncmp(g_mini.input, "echo", 4)))
		ft_echo();
	else if (!(ft_strncmp(g_mini.input, "cd", 2)))
		ft_cd();
	else if (!(ft_strncmp(g_mini.input, "pwd", 3)))
		ft_pwd();
	else if (!(ft_strncmp(g_mini.input, "export", 6)))
		ft_export();
	else if (!(ft_strncmp(g_mini.input, "unset", 5)))
		ft_unset();
	else if (!(ft_strncmp(g_mini.input, "env", 3)))
		ft_env();
	else if (!(ft_strncmp(g_mini.input, "exit", 4)))
		ft_exit();
	else
		ft_path();
}

//OBSOLETE function since getenv("PATH") does the trick

char	**ft_get_path(void)
{
	t_list	*temp;

	temp = g_mini.env;
	while (temp)
	{
		if (!ft_strncmp(temp->keyword, "PATH", 4))
			return (ft_split(temp->content, ':'));
		temp = temp->next;
	}
	return (NULL); // if there is not path then return error
}

// CREATING LIST //

t_list	*ft_env_list(char **env)
{
	int		i;
	t_list	*new;
	t_list	*head;
	t_list	*temp;

	new = NULL;
	head = NULL;
	temp = g_mini.env;
	i = -1;
	while (env[++i])
	{
		new = ft_lstnew(ft_split(env[i], '='));
		if (!new)
		{
			ft_lstclear(&head, free);
			return (NULL);
		}
		if (!head)
			head = new;
		ft_lstadd_back(&temp, new);
	}
	return (head);
}

/*
*	Initialize the struct
	-> ft_memset => sets all the variables of the struct to 0
*/

void	ft_init(char **argv, char **env)
{
	char	*temp_prompt;

	ft_memset(&g_mini, 0, sizeof(g_mini));
	temp_prompt = ft_strtrim(argv[0], "./");
	g_mini.prompt = ft_strjoin(temp_prompt, "42: ");
	g_mini.fd = 1;
	if (!g_mini.prompt)
		ft_error_exit("Error creating prompt string");
	ft_strdel(&temp_prompt);
	g_mini.env = ft_env_list(env);
	if (!g_mini.env)
		ft_error_exit("Malloc error while creating env list");
	g_mini.path = ft_get_path();
	if (!g_mini.path)
		ft_error_exit("No PATH variable found");
	if (tcgetattr(STDIN_FILENO, &g_mini.term) != 0)
		ft_error_exit( "Error getting terminal settings");
	g_mini.term.c_lflag &= ~ECHOCTL;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &g_mini.term) != 0)
		ft_error_exit( "Error setting terminal settings");
}

/*
**	Memory leak protected version of gnl with readline
	-> Clears previously allocated memory for 'line' before reading again
	-> Reads from user with the given prompt
	-> IF it's a valid line
		-> Add it to the line history (ignoring empty lines)
	-> RETURN the read line;
*/

char	*rl_gnl(void)
{
	static char	*line;

	if (line) 
		ft_strdel(&line);
	line = readline(g_mini.prompt);
	if (!line)
		exit(ft_clear_data(B));
	// if (!*line)
	// 	exit(ft_clear_data(g_mini, B));
	if (line != NULL && line[0] != 0)
		add_history(line);
	return (line);
}

int	main(int argc, char **argv, char **env)
{
	ft_init(argv, env);
	int k;

	k = -1;
	//signal(SIGINT, ft_handler);
	//signal(SIGQUIT, ft_handler);
	while (argc)
	{
		g_mini.input = rl_gnl();
		//g_mini.argv = ft_arguments(g_mini.input);
		g_mini.argv = ft_split_updated(g_mini.input, ' ');
		while (g_mini.argv[++k])
			printf("%s\n", g_mini.argv[k]);
		//g_mini.argv = ft_update_arg(g_mini.input);
		//the escape chars + single/double quotes need to be handled
		functions();
	}
}
