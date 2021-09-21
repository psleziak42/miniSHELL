#include "../extras/hfiles/minishell.h"

void	ft_cd(void)
{
	char	buf[4096];

	if (g_mini.argv[1][0] == '~')
		chdir(ft_lstfind_content(&g_mini.env, "HOME"));
	else
		if (chdir(g_mini.argv[1]) == -1)
			g_mini.exit_code = ft_error_handler("wrong chdir");
	getcwd(buf, 100);
	ft_lstfind_update(&g_mini.env, buf, "PWD");
}
