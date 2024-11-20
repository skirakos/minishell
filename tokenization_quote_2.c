/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization_quote_2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artyavet <artyavet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 22:56:28 by artyavet          #+#    #+#             */
/*   Updated: 2024/11/20 23:08:14 by artyavet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	quote_remover_helper(t_split *item, t_quote_var *var, char *result)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(item->value);
	while (i < len)
	{
		if (item->value[i] == '\'' && !var->in_double_quote)
			var->in_single_quote = !var->in_single_quote;
		else if (item->value[i] == '"' && !var->in_single_quote)
			var->in_double_quote = !var->in_double_quote;
		else
			result[var->j++] = item->value[i];
		i++;
	}
	result[var->j] = '\0';
}

int	quote_check(t_minishell *minishell, int *i, t_vars *vars)
{
	while (vars->input[*i] && vars->input[*i] != vars->current_quote)
		(*i)++;
	if (vars->input[*i] != vars->current_quote)
	{
		perror_exit(minishell, QUOTE_ERR, NULL, 0);
		return (1);
	}
	return (0);
}

void	init_quote_var(t_quote_var *var)
{
	var->j = 0;
	var->in_single_quote = 0;
	var->in_double_quote = 0;
}

int	quote_remover(t_split *item)
{
	char		*result;
	t_quote_var	var;

	while (item && item->value)
	{
		result = malloc(ft_strlen(item->value) + 1);
		if (!result)
			return (1);
		init_quote_var(&var);
		quote_remover_helper(item, &var, result);
		free(item->value);
		item->value = malloc(ft_strlen(result) + 1);
		if (!item->value)
			return (1);
		ft_strcpy(item->value, result);
		free(result);
		item = item->next;
	}
	return (0);
}
