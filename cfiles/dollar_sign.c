#include "../extras/hfiles/minishell.h"

/*
	Every strjoin call needs a free
*/

void	ft_dollar_sign(int i)
{
	t_list	*temp;

	temp = g_mini.env;
	while (temp)
	{
		if (!ft_strncmp(temp->keyword,
				g_mini.argv[i] + 1, ft_strlen(g_mini.argv[0] + 1)))
			ft_putstr_fd(temp->content, 1);
		temp = temp->next;
	}
}

/*
**	STILL NEED TO IMPLEMENT: a way to print what is before the $
	=> EXAMPLE: ben$PATH => ben + expanded PATH
	=> Only in "" !
*/

void	ft_expand_var()
{
	int	i;

	i = -1;
	while (g_mini.argv[++i])
		ft_dollar_sign(i);
}
