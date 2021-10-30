#include "minishell.h"

int main()
{
	pid_t	process;
	char	a;

	process = fork();
	if (process == -1)
		printf("a");	
	//ft_error_handler("fork error");
	else if (process == 0)
	{
		execve(g_mini.argv->full_arg_path, g_mini.argv->args, NULL);
		//ft_error_handler("execve error");
	}
		
	else
	{
		wait (NULL);
	}
		
}
