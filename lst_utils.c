#include "minishell.h"

t_split	*ft_lstnew(char *content, char *type)
{
	t_split	*n;

	n = malloc(sizeof(t_split));
	if (!n)
		return (NULL);
	n->value = content;
	n->type = type;
	n->next = NULL;
	return (n);
}