#include "../extras/hfiles/minishell.h"

int	ft_clear_data(t_minishell *mini, char c)
{
	int	i;

	i = -1;
	if (mini->path && c | B)
	{
		while (mini->path[++i])
			free(mini->path[i]);
		free(mini->path);
	}
	i = -1;
	if (mini->argv && (c | A || c | B))
	{
		while (mini->argv[++i])
			free(mini->argv[i]);
		free(mini->argv);
	}
	return (0);
}
