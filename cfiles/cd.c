#include "../extras/hfiles/minishell.h"

void	ft_update_pwd(char *cwd)
{
	t_list	*temp;

	temp = g_mini.env;
	while (temp)
	{
		if (!ft_strncmp(temp->keyword, "PWD", 3))
		{
			temp->content = ft_strjoin(ft_strjoin(temp->content, "/"), cwd);
			break;
		}
		temp = temp->next;
	}
	//free strjoin?
}

char	*go_up(char *cwd)
{
	int i;
	int	j;
	char *root;

	i = -1;
	j = -1;
	while (cwd[++i])
		;
	while (i)
	{
		if (cwd[i] == '/')
			break;
		i--;
	}
	root = malloc((i + 1) * sizeof(char));
	/*if (!root)
		ft_error*/
	while(++j < i)
		root[j] = cwd[j];	
	root[j] = '\0';
	printf("go up %s\n", root);
	return (root);
}

/*
 *	I set slash to 6 to escape norminette, it will count to 3 again, 3 + 3 = 6; 
 *
*/

char	*root(char *cwd)
{
	int		slash;
	int		i;
	char	*root;

	slash = 0;
	i = -1;
	while (cwd[++i])
	{
		if (cwd[i] == '/')
			slash++;
		if (slash == 3)
			break;
	}
	root = malloc((i + 1) * sizeof(char));
	i = -1;
	while (cwd[++i])
	{
		if (cwd[i] == '/')
			slash++;
		if (slash == 6)
			break;
		root[i] = cwd[i];
	}
	root[i] = '\0';
	printf("root = %s\n", root);
	return (root);
}

void	ft_cd(void)
{
	char	buf[4096];
	char	cwd[4096];

	getcwd(buf, 4096);
	if (!ft_strncmp(g_mini.argv[1], "~", 1))	
		ft_strlcpy(cwd, root(buf), ft_strlen(root(buf)));
	else if (!ft_strncmp(g_mini.argv[1], "..", 2))
		ft_strlcpy(cwd, go_up(buf), ft_strlen(go_up(buf)));
	else
	{
		ft_strlcpy(cwd, g_mini.argv[1], ft_strlen(g_mini.argv[1]));		
		ft_strjoin(buf, cwd);
	}
	printf("cwd: %s\n", buf);
	printf("strlen: %zu\n", ft_strlen(g_mini.argv[1]));
	if (chdir(buf) == -1)
	{
		//ft_error_handler // handla errno
		// return prompt //
		return ;
	}
	ft_update_pwd(cwd);
}

/*

	printf("%s\n", getcwd(buf, 4096));
	temp = g_mini.env;
	slash = 0;
	i = -1;
	if (!ft_strncmp(g_mini.argv[1], "~", 1))
	{
		getcwd(buf, 4096);
		while (buf[++i])
		{
			if (buf[i] == '/')
			{
				if (slash == 2)
					break;
				slash++;
			}
			root[i] = buf[i];
		}
		root[i] = '\0';
		chdir(root);
	}
	else if (!ft_strncmp(g_mini.argv[1], "..", 2))
		cwd(cwd(buf, 4096));
	else if (chdir(g_mini.argv[1]) == -1)
	{
		ft_error_handler("Builtin CD", ENOENT);
		return ;
	}
}*/
