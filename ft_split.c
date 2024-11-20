/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skirakos <skirakos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 15:56:04 by skirakos          #+#    #+#             */
/*   Updated: 2024/11/20 14:19:50 by skirakos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strncpy(char *s1, char *s2, int n)
{
	int	i;

	i = -1;
	while (++i < n && s2[i])
		s1[i] = s2[i];
	s1[i] = '\0';
	return (s1);
}

char	**start_malloc(char *str, char **out, int j, char c)
{
	int		k;
	int		i;

	k = 0;
	i = 0;
	out = (char **)malloc(sizeof(char *) * (j + 1));
	if (!out)
		return (NULL);
	while (str[i])
	{
		while (str[i] && (str[i] == c))
			i++;
		j = i;
		while (str[i] && (str[i] != c))
			i++;
		if (i > j)
		{
			out[k] = (char *)malloc(sizeof(char) * ((i - j) + 1));
			if (!out[k])
				return (free_matrix(out, matrix_len(out)));
			ft_strncpy(out[k++], &str[j], i - j);
		}
	}
	out[k] = NULL;
	return (out);
}

char	**ft_split(char *str, char c)
{
	int		i;
	int		j;
	int		wc;
	char	**out;

	i = 0;
	wc = 0;
	out = NULL;
	while (str[i])
	{
		while (str[i] && (str[i] == c))
			i++;
		if (str[i])
			wc++;
		while (str[i] && (str[i] != c))
			i++;
	}
	j = wc;
	return (start_malloc(str, out, j, c));
}
