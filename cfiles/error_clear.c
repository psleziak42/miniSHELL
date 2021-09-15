#include "../extras/hfiles/minishell.h"

/*
 * Clear all data from the struct
	1.	Clear the env list
	2.	Clear the path ** if it's != NULL && B is specified
	3.	Clear the argv ** if it's != NULL && A OR B is specified
	4.	Free && NULL the prompt string since it has allocated memory from init
*/

int	ft_clear_data(t_minishell *mini, char c)
{
	int	i;

	ft_lstclear(&mini->env, free);
	i = -1;
	if (mini->path && c | B)
	{
		while (mini->path[++i])
			ft_strdel(&mini->path[i]);
		free(mini->path);
	}
	i = -1;
	if (mini->argv && (c | A || c | B))
	{
		while (mini->argv[++i])
			ft_strdel(&mini->argv[i]);
		free(mini->argv);
	}
	ft_strdel(&mini->prompt);
	return (0);
}

/*
 * Error codes still need to be figured out
*/

void	ft_error_exit(t_minishell *mini, const char *errmessage)
{
	ft_clear_data(mini, B);
	perror(errmessage);
	exit(EXIT_FAILURE);
}

int	ft_error_handler(const char *errmessage, int err_code)
{
	perror(errmessage);
	return (err_code);
}
