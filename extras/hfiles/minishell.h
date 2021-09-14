/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosters <bcosters@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/10 14:52:05 by bcosters          #+#    #+#             */
/*   Updated: 2021/09/14 15:00:13 by psleziak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <stdio.h>
# include <unistd.h>
# include <signal.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <dirent.h>
# include <sys/ioctl.h>
# include <sys/errno.h>
# include <termios.h>
# include <curses.h>
# include <term.h>

typedef struct s_minishell
{
	char	*input;
	t_list	*env;
	char	**path;
	char	**argv;
	//pid_t	pid;
}				t_minishell;

//suggestion for updated linked list
/*typedef struct s_env
{
	char				*keyword;
	char				*content;
	struct s_envvars	*next;
}				t_env;*/

void	ft_echon(t_minishell *mini);
void	ft_echo(t_minishell *mini);
void	ft_cd(t_minishell *mini);
void	ft_pwd(t_minishell *mini);
void	ft_env(t_minishell *mini);
void	ft_exit(t_minishell *mini);
void	ft_export(t_minishell *mini);
void	ft_path(t_minishell *mini);

void	ft_handler(int signal);

#endif
