#ifndef MINISHELL_H
# define MINISHELL_H

#include "../libft/libft.h"
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

typedef struct	s_minishell
{
	char	*input;	
	t_list	*env;
	char	**PATH;
	char	**argv;
	pid_t	pid;
}				t_minishell;


void	ft_echon(t_minishell *mini);
void	ft_echo(t_minishell *mini);
void	ft_cd(t_minishell *mini);
void	ft_pwd(t_minishell *mini);
void	ft_env(t_minishell *mini);
void	ft_exit(t_minishell *mini);
void	ft_PATH(t_minishell *mini);

#endif
