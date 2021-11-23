/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_builtins_struct.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosilva <tosilva@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/28 15:41:19 by tosilva           #+#    #+#             */
/*   Updated: 2021/10/28 19:13:27 by tosilva          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	void	fill_cmd(void)
{
	g_mini.builtins.cmd = ft_calloc(8, sizeof(char *));
	g_mini.builtins.cmd[0] = ft_strdup("echo");
	g_mini.builtins.cmd[1] = ft_strdup("cd");
	g_mini.builtins.cmd[2] = ft_strdup("pwd");
	g_mini.builtins.cmd[3] = ft_strdup("env");
	g_mini.builtins.cmd[4] = ft_strdup("export");
	g_mini.builtins.cmd[5] = ft_strdup("unset");
	g_mini.builtins.cmd[6] = ft_strdup("exit");
}

static	void	fill_builtins_func(void)
{
	g_mini.builtins.builtin_func = ft_calloc(8, sizeof(char *));
	g_mini.builtins.builtin_func[0] = &ft_echo;
	g_mini.builtins.builtin_func[1] = &ft_cd;
	g_mini.builtins.builtin_func[2] = &ft_pwd;
	g_mini.builtins.builtin_func[3] = &ft_env;
	g_mini.builtins.builtin_func[4] = &ft_export;
	g_mini.builtins.builtin_func[5] = &ft_unset;
	g_mini.builtins.builtin_func[6] = &ft_exit;
}

void	fill_builtins_struct(void)
{
	fill_cmd();
	fill_builtins_func();
}
