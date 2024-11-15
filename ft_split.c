/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artyavet <artyavet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 15:56:04 by skirakos          #+#    #+#             */
/*   Updated: 2024/11/13 16:28:08 by artyavet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	*free_all(char **out)
// {
// 	int		i;

// 	i = 0;
// 	while (out[i])
// 	{
// 		free(out[i++]);
// 	}
// 	free(out);
// 	out = NULL;
// 	return (NULL);
// }

char	*ft_strncpy(char *s1, char *s2, int n)
{
	int	i;

	i = -1;
	while (++i < n && s2[i])
		s1[i] = s2[i];
	s1[i] = '\0';
	return (s1);
}

char	**start_malloc(char *str, char **out, int wc, int j, char c)
{
	int		k;
	int		i;

	k = 0;
	i = 0;
	out = (char **)malloc(sizeof(char *) * (wc + 1));
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
	j = 0;
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
	return (start_malloc(str, out, wc, j, c));
}

// int main()
// {
//     char str[] = "";
//     char **split = ft_split(str);

//     if (split)
//     {
//         for (int i = 0; split[i]; i++)
//         {
//             printf("split[%d]: %s\n", i, split[i]);
//         }

//         free_all(split);
//     }
//     else
//     {
//         printf("Memory allocation failed.\n");
//     }

//     return 0;
// }