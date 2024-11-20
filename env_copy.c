/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_copy.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artyavet <artyavet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 13:28:19 by skirakos          #+#    #+#             */
/*   Updated: 2024/11/20 23:12:17 by artyavet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_value(t_env **copy, char **env, int i, int *j)
{
	int	k;

	while (env[i][*j])
	{
		(*copy)->value = (char *)malloc(sizeof(char)
				* (ft_strlen(env[i]) - ft_strlen((*copy)->var) + 2));
		if (!(*copy)->value)
			return (1);
		k = 0;
		while (env[i][*j])
		{
			(*copy)->value[k] = env[i][*j];
			k++;
			(*j)++;
		}
		(*copy)->value[k] = '\0';
	}
	return (0);
}

t_env	*ft_lstnew_env(char *content, char *type)
{
	t_env	*n;

	n = malloc(sizeof(t_env));
	if (!n)
		return (NULL);
	n->var = content;
	n->value = type;
	n->next = NULL;
	return (n);
}

int	env_copy_helper_2(t_env *copy, char **env, int i, int *j)
{
	int	k;

	k = 0;
	while (env[i][*j] != '=')
		(*j)++;
	if (env[i][*j] == '=')
	{
		copy->var = (char *)malloc(sizeof(char) * ((*j) + 1));
		if (!copy->var)
			return (1);
		k = 0;
		while (k < (*j))
		{
			copy->var[k] = env[i][k];
			k++;
		}
		copy->var[k] = '\0';
	}
	return (0);
}

int	env_copy_helper_1(t_env *copy, char **env)
{
	int	i;
	int	j;

	i = 0;
	while (env[i])
	{
		j = 0;
		copy->next = ft_lstnew_env(NULL, NULL);
		if (!copy->next)
			return (1);
		copy = copy->next;
		if (env_copy_helper_2(copy, env, i, &j))
			return (1);
		j++;
		if (get_value(&copy, env, i, &j))
			return (1);
		i++;
	}
	return (0);
}

t_env	*env_copy(char **env)
{
	t_env	*copy;
	t_env	*tmp;

	copy = ft_lstnew_env(NULL, NULL);
	if (!copy)
		return (NULL);
	tmp = copy;
	if (env_copy_helper_1(copy, env))
	{
		free_t_env(tmp);
		return (NULL);
	}
	copy = tmp->next;
	free(tmp);
	return (copy);
}
