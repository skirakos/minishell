/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skirakos <skirakos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 21:28:07 by artyavet          #+#    #+#             */
/*   Updated: 2024/11/19 18:05:03 by skirakos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_underscore(char *cmd, int *i)
{
	if (cmd[0] == '_' && (cmd[1] == '\0' || cmd[1] == '='))
	{
		(*i)++;
		return (1);
	}
	return (0);
}

void	fook(char *str, char **res, int i)
{
	int	j;

	j = 0;
	if (str[i] && str[i] == '=')
	{
		i++;
		while (str[i])
		{
			res[1][j] = str[i++];
			j++;
		}
		res[1][j] = '\0';
	}
	if (j == 0)
	{
		free(res[1]);
		res[1] = NULL;
	}
}

void	for_copy(char *str, char **res)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '=')
	{
		res[0][i] = str[i];
		i++;
	}
	res[0][i] = '\0';
	fook(str, res, i);
	res[2] = NULL;
}

void	export_helper(t_minishell *minishell, char **split,
	char *new_value, int i)
{
	t_env	*tmp;

	tmp = minishell->env;
	if (contains_char(split[0], '+'))
		export_helper2(minishell, split, tmp, new_value);
	else if (matrix_len(split) == 1 && check_in_env(minishell->env, split[0]))
		export_helper4(minishell, tmp, split, i);
	else if (matrix_len(split) == 1 && !check_in_env(minishell->env, split[0]))
		export_helper5(minishell, tmp, split, i);
	else if (matrix_len(split) == 2
		&& check_in_env(minishell->env, split[0]) == 1)
		export_helper6(tmp, split);
	else if (matrix_len(split) == 2
		&& check_in_env(minishell->env, split[0]) == 0)
	{
		while (tmp->next && ft_strcmp(split[0], tmp->var) != 0)
			tmp = tmp->next;
		if (tmp->value)
			free(tmp->value);
		tmp->value = ft_strdup(split[1]);
		tmp = tmp->next;
	}
}
