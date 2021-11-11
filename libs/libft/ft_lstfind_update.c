/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstfind_update.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psleziak <psleziak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/11 18:40:30 by psleziak          #+#    #+#             */
/*   Updated: 2021/11/11 18:40:31 by psleziak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_lstfind_update(t_list **lst, char *cwd, char *find)
{
	t_list	*temp;

	temp = *lst;
	while (temp)
	{
		if (!ft_strncmp(temp->keyword, find, ft_strlen(find)))
		{
			free(temp->content);
			temp->content = ft_strdup(cwd);
			return (1);
		}
		temp = temp->next;
	}
	return (0);
}
