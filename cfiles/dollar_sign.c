#include "../extras/hfiles/minishell.h"

/*int	ft_my_strlen(char *argv)
{
	int i;

	i = -1;
	while (argv[++i])
		;
	return (i - 1);
}*/

/*
	Every strjoin call needs a free
*/

int	ft_dollar_sign(t_minishell *mini, int i)
{
	t_list	*temp;

	temp = mini->env;
	while (temp)
	{
		if (!ft_strncmp(temp->keyword,
				mini->argv[i] + 1, ft_strlen(mini->argv[0] + 1)))
		{
			ft_putstr_fd(temp->content, 1);
			return (1);
		}
		temp = temp->next;
	}
	return (0);
}

/*
**	STILL NEED TO IMPLEMENT: a way to print what is before the $
	=> EXAMPLE: ben$PATH => ben + expanded PATH
	=> Only in "" !
*/

int	ft_expand_var(t_minishell *mini)
{
	int	i;

	i = -1;
	while (mini->argv[++i])
	{
		ft_dollar_sign(mini, i);
	}
}