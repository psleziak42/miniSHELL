/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psleziak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/18 00:11:09 by psleziak          #+#    #+#             */
/*   Updated: 2021/05/20 23:48:44 by psleziak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	int		i;
	char	*new;

	if (!s)
		return (0);
	new = (char *)malloc(ft_strlen(s) * sizeof(char) + 1);
	if (!new)
		return (0);
	i = -1;
	while (s[++i] != '\0')
		new[i] = f(i, s[i]);
	new[i] = '\0';
	return (new);
}
