#include "../extras/hfiles/minishell.h"

/*int	ft_my_strlen(char *argv)
{
	int i;
	
	i = -1;
	while (argv[++i])
		;
	return (i - 1);
}*/



int	ft_dollar_sign(t_minishell *mini)
{
	t_list *temp;

	temp = mini->env;
	while (temp)
	{
		if (!ft_strncmp(ft_strjoin("$", temp->keyword), mini->argv[0], ft_strlen(mini->argv[0])))
		{
			ft_putstr_fd(temp->content, 1);
			return (1);
		}
		temp = temp->next;
	}
	return (0);
}
