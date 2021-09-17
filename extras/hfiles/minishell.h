/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosters <bcosters@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/10 14:52:05 by bcosters          #+#    #+#             */
/*   Updated: 2021/09/16 23:27:10 by psleziak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <stdio.h>
# include <unistd.h>
# include <signal.h>
# include <string.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/errno.h>
# include <dirent.h>
# include <sys/ioctl.h>
# include <sys/errno.h>
# include <termios.h>
# include <curses.h>
# include <term.h>

# define A	0b00000001 // A and B, look on it
# define B	0b00000010

typedef struct s_minishell
{
	char			*input;
	t_list			*env;
	char			**path;
	char			**argv;
	struct termios	term;
	char			*prompt;
	char			*cwd;
	unsigned char	exit_code;
}				t_minishell;

t_minishell	g_mini;

//suggestion for updated linked list
/*typedef struct s_env
{
	char				*keyword;
	char				*content;
	struct s_envvars	*next;
}				t_env;*/

void	ft_echon(void);
void	ft_echo(void);
void	ft_cd(void);
void	ft_pwd(void);
void	ft_export(void);
void	ft_unset(void);
void	ft_env(void);
void	ft_exit(void);
void	ft_path(void);

void	ft_handler(int signal);

/* ERROS AND CLEAN */

int		ft_clear_data(char c);
void	ft_error_exit(const char *errmessage);
int		ft_error_handler(const char *errmessage, int err_code);

/* DOLLAR */

void	ft_dollar_sign(int i);
void	ft_expand_var();
#endif
