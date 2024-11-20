/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artyavet <artyavet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 22:15:46 by artyavet          #+#    #+#             */
/*   Updated: 2024/11/17 22:37:35 by artyavet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	export_helper2(t_minishell *minishell, char **split,
	t_env *tmp, char *new_value)
{
	split[0][ft_strlen(split[0]) - 1] = '\0';
	if (!check_in_env(minishell->env, split[0]))
	{
		while (ft_strcmp(tmp->next->var, split[0]) != 0)
			tmp = tmp->next;
		if (!ft_strcmp(tmp->next->value, "\"\"") && split[1])
		{
			free(tmp->next->value);
			tmp->next->value = NULL;
		}
		new_value = ft_strjoin(tmp->next->value, split[1]);
		free(tmp->next->value);
		tmp->next->value = new_value;
		tmp->next->next = NULL;
		tmp = tmp->next;
	}
	else
		export_helper3(tmp, split);
}

void	export_helper3(t_env *tmp, char **split)
{
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = malloc(sizeof(t_env));
	tmp->next->var = ft_strdup(split[0]);
	tmp->next->value = ft_strdup(split[1]);
	tmp->next->next = NULL;
	tmp = tmp->next;
}

void	export_helper4(t_minishell *minishell, t_env *tmp, char **split, int i)
{
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = malloc(sizeof(t_env));
	tmp->next->var = ft_strdup(split[0]);
	if (contains_char(minishell->cmd[i], '=') == 1)
		tmp->next->value = ft_strdup("\"\"");
	else
		tmp->next->value = NULL;
	tmp->next->next = NULL;
	tmp = tmp->next;
}

void	export_helper5(t_minishell *minishell, t_env *tmp, char **split, int i)
{
	while (ft_strcmp(tmp->next->var, split[0]) != 0)
		tmp = tmp->next;
	if (contains_char(minishell->cmd[i], '=') == 1)
	{
		free(tmp->next->value);
		tmp->next->value = ft_strdup("\"\"");
	}
	tmp->next->next = NULL;
	tmp = tmp->next;
}

void	export_helper6(t_env *tmp, char **split)
{
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = malloc(sizeof(t_env));
	tmp->next->var = ft_strdup(split[0]);
	tmp->next->value = ft_strdup(split[1]);
	tmp->next->next = NULL;
	tmp = tmp->next;
}
