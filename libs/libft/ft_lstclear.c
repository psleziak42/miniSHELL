/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosters <bcosters@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/17 23:21:11 by psleziak          #+#    #+#             */
/*   Updated: 2021/09/15 11:57:45 by bcosters         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	struct s_list	*temp;
	struct s_list	*previous;

	temp = *lst;
	previous = *lst;
	while (temp != NULL)
	{
		previous = temp->next;
		del(temp->keyword);
		del(temp->content);
		free(temp);
		temp = previous;
	}
	*lst = NULL;
}
