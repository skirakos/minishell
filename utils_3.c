/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skirakos <skirakos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 13:34:56 by skirakos          #+#    #+#             */
/*   Updated: 2024/11/20 13:34:57 by skirakos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_split	*ft_lstnew(char *content)
{
	t_split	*n;

	n = malloc(sizeof(t_split));
	if (!n)
		return (NULL);
	n->value = content;
	n->next = NULL;
	return (n);
}

int	ft_strlen(char *str)
{
	int i;

	i = 0;
	if (!str)
	{
		return (0);
	}
	while (str[i])
	{
		i++;
	}
	return (i);
}

void	ft_strcut(t_split *item, char *input, int start, int end)
{
	int	i;

	i = 0;
	item->value = malloc(end - start + 2);
	if (!item->value)
		return ;
	while (input[i] && start <= end)
	{
		item->value[i] = input[start];
		i++;
		start++;
	}
	item->value[i] = '\0';
	item->type = WORD;
}

char	*ft_substr(char *s, int start, int end)
{
	int		i;
	int		j;
	char	*arr;

	j = 0;
	if (!s)
		return (NULL);
	if (start >= ft_strlen(s))  // If start is beyond the length of the string
		i = 1;
	else if (ft_strlen(s) < end)  // If end exceeds the string length
		i = ft_strlen(s) - start + 1;
	else
		i = end - start + 1;

	arr = (char *)malloc(sizeof(char) * i);
	if (!arr)
		return (NULL);
	while (i - 1)
	{
		arr[j] = s[j + start];
		j++;
		i--;
	}
	arr[j] = '\0';
	return (arr);
}
