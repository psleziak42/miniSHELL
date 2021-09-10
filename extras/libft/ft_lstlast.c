/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstlast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psleziak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/17 23:47:44 by psleziak          #+#    #+#             */
/*   Updated: 2021/05/17 23:53:43 by psleziak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstlast(t_list *lst)
{
	struct s_list	*previous;
	struct s_list	*current;

	previous = lst;
	current = lst;
	if (lst == NULL)
		return (lst);
	while (current != NULL)
	{
		previous = current;
		current = current->next;
	}
	return (previous);
}
