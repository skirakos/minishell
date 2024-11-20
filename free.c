/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skirakos <skirakos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 18:05:27 by skirakos          #+#    #+#             */
/*   Updated: 2024/11/19 18:07:45 by skirakos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_t_split(t_split *tokens)
{
	t_split	*tmp;

	tmp = NULL;
	while (tokens)
	{
		tmp = tokens;
		tokens = tokens->next;
		tmp->next = NULL;
		free(tmp);
		tmp = NULL;
	}
	free(tokens);
}

void	free_t_env(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env;
		free(env->var);
		free(env->value);
		env = env->next;
		free(tmp);
	}
	free(env);
}

int	matrix_len(char **matrix)
{
	int	i;

	i = 0;
	while (matrix[i])
		i++;
	return (i);
}

void	*free_matrix(char **matrix, int rows)
{
	int	i;

	i = 0;
	while (i < rows)
	{
		free(matrix[i]);
		i++;
	}
	free(matrix);
	return (NULL);
}

void	free_before_exit(t_minishell *minishell)
{
	if (minishell->tokens)
		free_t_split(minishell->tokens);
	if (minishell->env)
		free_t_env(minishell->env);
	if (minishell->pid)
		free(minishell->pid);
	if (minishell->cmd)
		minishell->cmd = free_matrix(minishell->cmd,
				matrix_len(minishell->cmd));
	if (minishell->fd)
		free(minishell->fd);
	if (minishell)
		free(minishell);
	minishell = NULL;
}
