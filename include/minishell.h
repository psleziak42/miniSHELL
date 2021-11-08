/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psleziak <psleziak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/10 14:52:05 by psleziak          #+#    #+#             */
/*   Updated: 2021/11/08 17:54:55 by psleziak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <stdio.h>
# include <unistd.h>
# include <signal.h>
# include <string.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/errno.h>
# include <sys/wait.h>
# include <dirent.h>
# include <sys/ioctl.h>
# include <sys/errno.h>
# include <termios.h>
# include <curses.h>
# include <term.h>

# define INVALID_COMMAND	"command not found"

typedef struct s_arguments
{
	char				**args;
	char				pipe_type[4];
	char				*full_arg_path;
	bool				is_valid;
	struct s_arguments	*next;
}				t_arguments;

typedef struct s_builtins
{
	char	**cmd;
	void	(**builtin_func)(char **);
}				t_builtins;


/**
 * Note: "t_builtins builtins" is not a pointer becaus
**/
typedef struct s_minishell
{
	char			*input;
	char			**path;
	t_list			*env;
	t_arguments		*argv;
	t_builtins		builtins;
	t_quote			quote;
	char			*prompt;
	char			*cwd;
	int				fd;
	struct termios	term;
	unsigned char	exit_code;
}				t_minishell;

t_minishell	g_mini;

void		ft_echo(char **args);
void		ft_cd(char **args);
void		ft_pwd(char **args);
void		ft_export(char **args);
void		ft_unset(char **args);
void		ft_env(char **args);
void		ft_exit(char **args);
void		ft_path(char **args);

void		ft_handler(int signal);

/* PARSER FUNCTIONS */
int			is_pipe(char *str);
int			get_var_info(char *var_name, char **kw_content, int *content_len);
char		*ft_expand_dollar(int *input_i);
char		*ft_expand_tilde(int *input_i);
t_arguments	*split_commands(t_arguments *old_argv);

/* ERRORS AND CLEAN */
int			ft_clear_data(void);
void		ft_error_exit(const char *errmessage);
void		ft_cmd_error_handler(char *command, char *argumment, char *description);
void		ft_default_error_handler(char *message, char *description);

/* DOLLAR */
void		ft_dollar_sign(char *argv);
void		ft_expand_var(char *argv);

/* LINKED LIST EXTRAS */
void		add_to_end_of_the_list(t_arguments **all_args,
				t_arguments *new_arg);
void		ft_free_args(t_arguments *old_argv);

/* ACCESS and PIPES*/
void		fill_builtins_struct(void);
void		check_commands(void);
void		run_commands(void);
int 		get_type_of_pipe(char *pipe_type);

#endif
