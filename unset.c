/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artyavet <artyavet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 13:41:51 by skirakos          #+#    #+#             */
/*   Updated: 2024/11/20 23:21:05 by artyavet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	unset_first_node(t_minishell *minishell, t_env *tmp, int *i)
{
	if (ft_strcmp(minishell->cmd[*i], minishell->env->var) == 0)
	{
		tmp = minishell->env->next;
		free(minishell->env->value);
		free(minishell->env->var);
		free(minishell->env);
		minishell->env = tmp;
		(*i)++;
		return (1);
	}
	return (0);
}

int	unset_node(t_minishell *minishell,
	t_env *node_to_delete, t_env **prev, int i)
{
	if (ft_strcmp(minishell->cmd[i], minishell->env->var) == 0)
	{
		node_to_delete = minishell->env;
		(*prev)->next = minishell->env->next;
		free(node_to_delete->var);
		free(node_to_delete->value);
		free(node_to_delete);
		return (1);
	}
	(*prev) = minishell->env;
	minishell->env = minishell->env->next;
	return (0);
}

void	unset_helper(t_minishell *minishell, t_env *tmp,
	t_env *prev, t_env *node_to_delete)
{
	int	i;

	i = 1;
	while (minishell->cmd[i])
	{
		if (check_underscore(minishell->cmd[i], &i))
			continue ;
		if (is_valid_var(minishell->cmd[i]))
			print_err(1, "minishell: unset: `", minishell->cmd[i],
				"': not a valid identifier\n");
		if (unset_first_node(minishell, tmp, &i))
			continue ;
		tmp = minishell->env;
		prev = minishell->env;
		minishell->env = minishell->env->next;
		while (minishell->env)
		{
			if (unset_node(minishell, node_to_delete, &prev, i))
				break ;
		}
		minishell->env = tmp;
		i++;
	}
}

void	unset(t_minishell *minishell)
{
	t_env	*tmp;
	t_env	*prev;
	t_env	*node_to_delete;

	if (!minishell->env || !minishell->cmd[1])
		return ;
	prev = NULL;
	tmp = NULL;
	node_to_delete = NULL;
	unset_helper(minishell, tmp, prev, node_to_delete);
}
