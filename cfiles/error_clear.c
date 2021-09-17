#include "../extras/hfiles/minishell.h"

/*
 * Clear all data from the struct
	1.	Clear the env list
	2.	Clear the path ** if it's != NULL && B is specified
	3.	Clear the argv ** if it's != NULL && A OR B is specified
	4.	Free && NULL the prompt string since it has allocated memory from init
*/

int	ft_clear_data(char c)
{
	int	i;

	ft_lstclear(&g_mini.env, free);
	i = -1;
	if (g_mini.path && c | B)
	{
		while (g_mini.path[++i])
			ft_strdel(&g_mini.path[i]);
		free(g_mini.path);
	}
	i = -1;
	if (g_mini.argv && (c | A || c | B))
	{
		while (g_mini.argv[++i])
			ft_strdel(&g_mini.argv[i]);
		free(g_mini.argv);
	}
	ft_strdel(&g_mini.prompt);
	return (0);
}

/*
 * Error codes still need to be figured out
*/

void	ft_error_exit(const char *errmessage)
{
	ft_clear_data(B);
	perror(errmessage);
	exit(EXIT_FAILURE);
}

int	ft_error_handler(const char *errmessage, int err_code)
{
	// get errno number //
	perror(errmessage);
	return (err_code);
}
