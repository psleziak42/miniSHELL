#include "../extras/hfiles/minishell.h"

/*
 *	$? works
 *	working on "" and ''
 *
 *	check if arg starts from "
 *	check if arg finishes on "
 *
 *	PROBLEMS:
 *		when counting characters for string split, " can start anywhere, not necesarly in the beginning..."
 *		doing it char by char, the problem is how to not pass last "
 *		if i pass last " then i have to strjoin " to linkedlist keyword. but " doesnt have to finish at the end of the word: $PATH"
 *
 *
*/

/*void	ft_double_quote(char *arg)
{
	while (g_mini.argv[*i][(*j)] != '\"')	
	{
		if (!g_mini.argv[*i][*j])
		{
			if (g_mini.argv[*i][(*j) - 1] == '\"')
				break ;	
			if (g_mini.argv[(*i) + 1])
			{
				ft_putchar_fd(' ', g_mini.fd);
				(*i)++;
				(*j) = 0;
				break ;
			}
			else
				break ;
		}
		if (g_mini.argv[*i][*j] == '$') // trzeba wyslac bez " i sprawdzic czy jest " 
		{
			ft_expand_var(g_mini.argv[*i]);
			(*i)++;
			(*j) = 0;
		}
		else
			ft_putchar_fd(g_mini.argv[*i][*j], g_mini.fd);
		(*j)++;
	}
}*/

void	ft_echo(void)
{
	int	i;
	int	j;
	int	q;
	int	x;
	int	double_quote;
	char	arg_dolar[100];
	char	arg_quote[200];

	i = 0;
	q = 0;
	x = 0;
	while (g_mini.argv[++i])
	{
		if (!ft_strncmp(g_mini.argv[i], "$?", 2))
		{
			printf("%d", g_mini.exit_code);
			continue ;
		}
		j = 0;
		if (g_mini.argv[i][j++] == '\"')
		{
			double_quote = 1;
			while (/*g_mini.argv[i][++j] &&*/ double_quote != 2)
			{
				if (!g_mini.argv[i][j])
				{
					//printf("end of workd\n");
					i++;
					j = 0;
					arg_quote[q++] = ' ';
					continue ;
				}
				if (g_mini.argv[i][j] == '\"')
				{
					//printf("double quote\n");
					double_quote++;	
					break ;
				}
				arg_quote[q++] = g_mini.argv[i][j]; // its missing to check if there is anything after ". To join it and print. then on the bottomelse i could use put string not char and have result.
				j++;
			}
			arg_quote[q] = '\0';
			//printf("arg_quote: %s, %d\n", arg_quote, q);
			q = -1;
			while(arg_quote[++q])
			{
				//printf("arg_quote %c[%d]\n", arg_quote[q], q);
				if (arg_quote[q] == '$')	
				{
					q++;
					while (arg_quote[q] != '\0' && arg_quote[q] != '\t' && arg_quote[q] != ' ')
						arg_dolar[x++] = arg_quote[q++];
					arg_dolar[x] = '\0';
					ft_dollar_sign(arg_dolar);
				}		
				else
					ft_putchar_fd(arg_quote[q], g_mini.fd);
			}
		}	
		else
			ft_putchar_fd(g_mini.argv[i][j], g_mini.fd);
		j++;
	}
	printf("\n");
}


/*
void	ft_echo(void)
{
	int i;
	int	j;

	i = 0;
	while (g_mini.argv[++i])
	{
		if (!ft_strncmp(g_mini.argv[i], "$?", 2))
		{
			printf("%d", g_mini.exit_code);
			continue ;
		}
		j = -1;
		//printf("g_mini.argv[%d] = %s\n", i, g_mini.argv[i]);
		while (g_mini.argv[i][++j])
		{
			*if (g_mini.argv[i][j] != '\'')
				ft_putchar_fd(g_mini.argv[i][j], 1);*
			if (g_mini.argv[i][j++] == '\"')
				ft_double_quote(ft_substr(g_mini.argv[i], j + 1, (ft_strlen(g_mini.argv[i]) - 3)));
		}
		//printf("%s", g_mini.argv[i]);
		if (g_mini.argv[i + 1])
			ft_putchar_fd(' ', 1);
	}
	printf("\n");
}*/
