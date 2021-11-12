/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psleziak <psleziak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/10 14:52:05 by psleziak          #+#    #+#             */
/*   Updated: 2021/11/12 17:34:27 by psleziak         ###   ########.fr       */
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

# define MALLOC_ERROR		"memory not allocated with malloc"
# define INVALID_COMMAND	"command not found"
# define SYNTAX_ERROR		"Syntax error near unexpected token"
# define NUM_REQUIRED		"numeric argument required"
# define TOO_MANY_ARGS		"too many arguments"
# define FEW_ARGS			"not enough arguments"

typedef enum e_exit_codes
{
	GENERAL_ERR			= 1,
	MISUSE_BUILTIN		= 2,
	CMD_CANT_EXEC		= 126,
	CMD_NOT_FOUND		= 127,
	INVALID_EXIT_ARG	= 128,
	CTRL_C				= 130,
	OUT_OF_RANGE		= 255,
	TOKEN_ERR			= 258
}				t_e_exit_codes;

typedef struct s_arguments
{
	char				**args;
	char				pipe_type[4];
	char				*cmd_w_path;
	bool				is_valid;
	struct s_arguments	*next;
}				t_arguments;

typedef struct s_quote
{
	int		on_quote;
	char	qt;
}				t_quote;

typedef struct s_builtins
{
	char	**cmd;
	void	(**builtin_func)(char **, int);
}				t_builtins;

/**
 * Note: "t_builtins builtins" is not a pointer becaus
**/
typedef struct s_minishell
{
	char			*input;
	char			**path;
	pid_t			pid_id;
	t_list			*env;
	t_arguments		*argv;
	t_builtins		builtins;
	t_quote			quote;
	char			*prompt;
	char			*cwd;
	int				fd;
	struct termios	term;
	bool			has_error;
	unsigned char	exit_code;
}				t_minishell;

t_minishell	g_mini;

void		ft_echo(char **args, int fd_out);
void		ft_cd(char **args, int fd_out);
void		ft_pwd(char **args, int fd_out);
void		ft_export(char **args, int fd_out);
void		ft_unset(char **args, int fd_out);
void		ft_env(char **args, int fd_out);
void		ft_exit(char **args, int fd_out);
void		ft_path(char **args, int fd_out);

void		ft_init(char **argv, char **env);
void		ft_handler(int signal);
char		**ft_get_path(void);

/* PARSER FUNCTIONS */
int			is_pipe(char *str);
int			is_closed_quote(int input_i);
int			copy_word_inside_quote(char *arg, char *str, int *i, int *word_len);
bool		is_tilde_to_home(int i, char *str);
bool		is_shell_var(char *var);
int			get_var_info(char *var_name, char **kw_content, int *content_len);
char		*ft_expand_dollar(int *input_i);
char		*ft_expand_tilde(int *input_i);
void		copy_args(t_arguments **new_arg, int *input_i, int nr_words);
void		ft_skip_spaces(int *input_i);
void		ft_go_to_next_cmd(int *input_i);
t_arguments	*split_commands(t_arguments *old_argv);

/* ERRORS AND CLEAN */
int			ft_clear_data(void);
void		error_exit(char *message, char *description, int exit_code);
void		cmd_error_handler(char *cmd, char *arg, char *description,
				int exit_code);
void		deflt_err_handler(char *message, char *description,
				int exit_code);

/* LINKED LIST EXTRAS */
void		add_to_end_of_the_list(t_arguments **all_args,
				t_arguments *new_arg);
void		ft_free_args(t_arguments *old_argv);

/* ACCESS and PIPES and REDIRECTIONS */
void		run_builtin_or_execve(char *cmd_w_path, char **args,
				int fd_in, int fd_out);
void		ft_input(int *in, t_arguments *temp);
void		ft_input_heredoc(int *in, t_arguments *temp);
void		ft_output(int *out, t_arguments *temp);
void		ft_ouptut_append(int *out, t_arguments *temp);
void		io_table_manipulation(int *in, int *out, t_arguments *temp,
				int pipe_type);
void		fill_builtins_struct(void);
void		check_commands(void);
void		run_pipe_or_single_cmd(void);
void		run_pipes(t_arguments *temp, int nr_of_commands);
bool		is_redirection(char *pipe_type);
int			get_type_of_pipe(char *pipe_type);

#endif
