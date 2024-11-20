/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization_env.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artyavet <artyavet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 22:50:51 by artyavet          #+#    #+#             */
/*   Updated: 2024/11/20 23:06:19 by artyavet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_from_env(t_env *env, char *key)
{
	char	*res;
	int		j;

	while (env)
	{
		if (!ft_strcmp(env->var, key))
		{
			res = (char *)malloc(sizeof(char *) * (ft_strlen(env->value) + 1));
			if (!res)
				return (NULL);
			j = 0;
			while (env->value[j])
			{
				res[j] = env->value[j];
				j++;
			}
			res[j] = '\0';
			return (res);
		}
		env = env->next;
	}
	return (NULL);
}

char	*merge(char *before_key, char *dollar_value, char *after_key)
{
	char	*tmp;
	char	*res;

	tmp = ft_strjoin(before_key, dollar_value);
	free(before_key);
	free(dollar_value);
	res = ft_strjoin(tmp, after_key);
	free(tmp);
	free(after_key);
	return (res);
}

int	check_str(char *str, int i)
{
	if (str[i] && (str[i] != ' '
			&& str[i] != '=' && str[i] != '/'
			&& str[i] != '$' && str[i] != '"'
			&& str[i] != 39 && str[i] != '>'
			&& str[i] != '<' && str[i] != '>'
			&& str[i] != '\0'))
		return (1);
	return (0);
}

int	check_key_in_env(t_env *env, char *key, t_split *item)
{
	int	i;

	while (env)
	{
		if (!ft_strncmp(env->var, key, ft_strlen(env->var)))
		{
			i = 0;
			while (item->value[i] && env->value[i])
			{
				item->value[i] = env->value[i];
				i++;
			}
			return (1);
		}
		env = env->next;
	}
	free(item->value);
	item->value = NULL;
	return (-1);
}
