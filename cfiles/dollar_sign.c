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

int	ft_dollar_sign(t_minishell *mini)
{
	t_list	*temp;
	char	*env_var;

	temp = mini->env;
	while (temp)
	{
		env_var = ft_strjoin("$", temp->keyword);
		if (!ft_strncmp(env_var, mini->argv[0], ft_strlen(mini->argv[0])))
		{
			ft_putstr_fd(temp->content, 1);
			ft_strdel(&env_var);
			return (1);
		}
		temp = temp->next;
		ft_strdel(&env_var);
	}
	return (0);
}
