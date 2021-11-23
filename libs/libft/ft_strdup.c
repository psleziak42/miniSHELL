/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psleziak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/17 22:14:12 by psleziak          #+#    #+#             */
/*   Updated: 2021/05/17 22:14:20 by psleziak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	int		i;
	char	*up;

	i = 0;
	while (s[i] != '\0')
		i++;
	up = (char *)malloc((i + 1) * sizeof(char));
	if (!up)
		return (0);
	i = 0;
	while (s[i] != '\0')
	{
		up[i] = s[i];
		i++;
	}
	up[i] = '\0';
	return (up);
}
