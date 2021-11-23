/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psleziak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/18 00:01:23 by psleziak          #+#    #+#             */
/*   Updated: 2021/09/13 22:25:27 by psleziak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(char **content)
{
	struct s_list	*head;

	head = malloc(sizeof(struct s_list));
	if (!head)
		return (0);
	head->keyword = content[0];
	head->content = content[1];
	head->next = NULL;
	return (head);
}
