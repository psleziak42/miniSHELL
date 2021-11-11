/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstfind_content.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psleziak <psleziak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/11 18:40:34 by psleziak          #+#    #+#             */
/*   Updated: 2021/11/11 18:40:35 by psleziak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_lstfind_content(t_list **lst, char *find)
{
	t_list	*temp;
	size_t	len;

	temp = *lst;
	len = ft_strlen(find);
	while (temp)
	{
		if (!ft_strncmp(temp->keyword, find, len + 1))
			return (temp->content);
		temp = temp->next;
	}
	return (NULL);
}
