#include "libft.h"

char	*ft_lstfind_content(t_list **lst, char *find)
{
	t_list	*temp;

	temp = *lst;
	while (temp)
	{
		if (!ft_strncmp(temp->keyword, find, ft_strlen(find)))
			return (temp->content);
		temp = temp->next;
	}
	return ("Element not found, check spelling");
}
