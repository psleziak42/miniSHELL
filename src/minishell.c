/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosters <bcosters@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/10 14:56:12 by bcosters          #+#    #+#             */
/*   Updated: 2021/10/21 19:00:47 by psleziak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//hello przemek

/*
**	Signal handler
	-> Ctrl-C == SIGINT
	-> Ctrl-\ == SIGQUIT
	-> Ctrl-D == EOF aka close inputstream or shell
*/

void	ft_handler(int sig)
{
	if (sig == SIGINT)
	{
		g_mini.exit_code = 130;
		printf("%s\n", g_mini.prompt);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	if (sig == SIGQUIT) // shouldnt move cursor
		printf("%s%s", g_mini.prompt, rl_line_buffer);
}

/*	 to do list by friday
 *-> access
 *-> signals and library connection
*/ 

short	check_valid_cmd(void)
{
	char	*full_cmd_path;
	char	*temp;
	size_t	i;

	i = -1;
	while (g_mini.path[++i])
	{
		temp = ft_strjoin(g_mini.path[i], "/");
		full_cmd_path = ft_strjoin(temp, g_mini.argv[0]);
		free(temp);
		if (access(full_cmd_path, F_OK | X_OK) == 0)
		{
			free(full_cmd_path);
			return (1);
		}
		free(full_cmd_path);
	}
	return (0);
}

void	functions(void)
{
//	if (g_mini.argv[0][0] == '$')
//		ft_expand_var(); // working for argv[1];
	
	if (!(ft_strncmp(g_mini.argv[0], "echo", 5)))
	{
		ft_echo();
		if (!(g_mini.argv[1] && !ft_strncmp(g_mini.argv[1], "-n", 3)))
			printf("\n");
	}
	else if (!(ft_strncmp(g_mini.argv[0], "cd", 3)))
		ft_cd();
	else if (!(ft_strncmp(g_mini.argv[0], "pwd", 4)))
		ft_pwd();
	else if (!(ft_strncmp(g_mini.argv[0], "export", 7)))
		ft_export();
	else if (!(ft_strncmp(g_mini.argv[0], "unset", 6)))
		ft_unset();
	else if (!(ft_strncmp(g_mini.argv[0], "env", 4)))
		ft_env();
	else if (!(ft_strncmp(g_mini.argv[0], "exit", 5)))
		ft_exit();
	else if (check_valid_cmd())
		ft_path();
	else
		ft_error_exit("error execve");
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

	new = NULL;
	head = NULL;
	i = -1;
	while (env[++i])
	{
		new = ft_lstnew(ft_split(env[i], '='));
		if (!new)
		{
			ft_lstclear(&head, free);
			return (NULL);
		}
		// if (!head)
		// 	head = new;
		ft_lstadd_back(&head, new);
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
	static char	*line_trimmed;
	char		*line;

	if (line_trimmed) 
		ft_strdel(&line_trimmed);
	line = readline(g_mini.prompt);
	if (!line)
	{
		//printf("%s\n", "exit");
		ft_clear_data();
		exit(1);
	}
	// if (!*line)
	// 	exit(ft_clear_data(g_mini, B));
	if (line != NULL && line[0] != 0)
		add_history(line);
	line_trimmed = ft_strtrim(line, " \t\v");
	ft_strdel(&line);
	return (line_trimmed);
}

int	main(int argc, char **argv, char **env)
{
	ft_init(argv, env);
	int k;


	signal(SIGINT, ft_handler);
	signal(SIGQUIT, ft_handler);
	while (argc)
	{
		g_mini.input = rl_gnl();
		if (ft_strncmp(g_mini.input, "", 1) == 0)
			continue ;
		//g_mini.argv = ft_arguments(g_mini.input);
		g_mini.argv = ft_split_updated(g_mini.input, ' ');
		// g_mini.argv = split_commands(g_mini.argv);
		if (!g_mini.argv)
			continue ;
		//g_mini.argv = ft_split(g_mini.input, ' ');
		k = -1;
		// while (g_mini.argv[++k])
		// 	printf("argv: %d %s\n", k, g_mini.argv[k]);
		//g_mini.argv = ft_update_arg(g_mini.input);
		//the escape chars + single/double quotes need to be handled
		functions();
	}
}
