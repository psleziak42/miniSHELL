/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstfind_update.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psleziak <psleziak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/11 18:40:30 by psleziak          #+#    #+#             */
/*   Updated: 2021/11/12 15:46:22 by psleziak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_lstfind_update(t_list **lst, char *cwd, char *find)
{
	t_list	*temp;
	size_t	len;

	len = ft_strlen(find);
	temp = *lst;
	while (temp)
	{
		if (!ft_strncmp(temp->keyword, find, len + 1))
		{
			free(temp->content);
			temp->content = ft_strdup(cwd);
			return (1);
		}
		temp = temp->next;
	}
	return (0);
}
