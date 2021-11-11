/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_initialization.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psleziak <psleziak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/11 17:30:24 by psleziak          #+#    #+#             */
/*   Updated: 2021/11/11 18:31:56 by psleziak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**ft_get_path(void)
{
	t_list	*temp;

	temp = g_mini.env;
	while (temp)
	{
		if (!ft_strncmp(temp->keyword, "PATH", 4))
			return (ft_split(temp->content, ':'));
		temp = temp->next;
	}
	return (NULL);
}

static t_list	*ft_env_list(char **env)
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

	ft_bzero(&g_mini, sizeof(g_mini));
	fill_builtins_struct();
	temp_prompt = ft_strtrim(argv[0], "./");
	g_mini.prompt = ft_strjoin(temp_prompt, "42: ");
	g_mini.fd = 1;
	if (!g_mini.prompt)
		error_exit("Fatal Error", "creating prompt string", GENERAL_ERR);
	ft_strdel(&temp_prompt);
	g_mini.env = ft_env_list(env);
	if (!g_mini.env)
		error_exit("Fatal Error", MALLOC_ERROR, GENERAL_ERR);
	g_mini.path = ft_get_path();
	if (!g_mini.path)
		error_exit("Fatal Error", "no PATH variable found", GENERAL_ERR);
	if (tcgetattr(STDIN_FILENO, &g_mini.term) != 0)
		error_exit("Fatal Error", "getting terminal settings", GENERAL_ERR);
	g_mini.term.c_lflag &= ~ECHOCTL;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &g_mini.term) != 0)
		error_exit("Fatal Error", "setting terminal settings", GENERAL_ERR);
}
