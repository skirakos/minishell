/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skirakos <skirakos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 19:24:01 by artyavet          #+#    #+#             */
/*   Updated: 2024/11/20 13:48:09 by skirakos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_digit(char c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

int	check_in_env(t_env *env, char *str)
{
	t_env	*tmp;

	tmp = env;
	while (env && env->var)
	{
		if (ft_strcmp(env->var, str) == 0)
			return (0);
		env = env->next;
	}
	env = tmp;
	return (1);
}

int	contains_char(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == c)
			return (1);
		i++;
	}
	return (0);
}

int	is_valid_var(char *var)
{
	int	i;

	i = 0;
	while (var[i] != '\0')
	{
		if (!((var[i] >= 'a' && var[i] <= 'z')
				|| (var[i] >= 'A' && var[i] <= 'Z')
				|| (var[i] >= '0' && var[i] <= '9')
				|| (var[i] == '+' && var[i + 1] == '\0')
				|| var[i] == '_'))
			return (1);
		i++;
	}
	return (0);
}
