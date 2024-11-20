/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artyavet <artyavet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 20:01:54 by artyavet          #+#    #+#             */
/*   Updated: 2024/11/20 23:25:27 by artyavet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_vars(t_vars *vars, char *input)
{
	vars->start = 0;
	vars->end = 0;
	vars->current_quote = 0;
	vars->input = input;
}

int	start_tokenisation(t_minishell *minishell,
	char *input, t_split *item, int i)
{
	t_vars	vars;

	init_vars(&vars, input);
	while (input && input[i])
	{
		if (find_opening_quote(minishell, &item, &i, &vars) == -1)
			return (1);
		else if (find_opening_quote(minishell, &item, &i, &vars) == 1)
			continue ;
		vars.flag = find_closing_quote(&item, &i, &vars);
		if (vars.flag == -1)
			return (1);
		else if (vars.flag == 1)
			continue ;
		if (vars.input[i] == '"' || vars.input[i] == 39)
		{
			vars.current_quote = vars.input[i];
			continue ;
		}
		vars.end = --i;
		if (create_item_next(&item, &vars) == 1)
			return (1);
		i++;
	}
	return (0);
}

int	tokenization_helper(t_split *item, t_minishell *minishell, t_split *tmp)
{
	tmp = item;
	if (item)
	{
		if (dollar_sign(item, minishell->env))
		{
			free_t_split(tmp);
			return (1);
		}
		check_operation(&item);
		tmp = item;
		if (syntax_check(minishell, item) == 1)
		{
			free_t_split(tmp);
			return (2);
		}
		item = tmp;
		minishell->tokens = item;
		if (minishell->tokens && minishell->tokens->value)
		{
			if (initialization(minishell))
				return (1);
			built_in(minishell, -1);
		}
	}
	return (0);
}

int	tokenization(char *input, t_minishell *minishell)
{
	t_split	*item;
	t_split	*tmp;

	item = ft_lstnew(NULL);
	if (!item)
		return (1);
	tmp = item;
	if (start_tokenisation(minishell, input, item, 0))
	{
		free_t_split(tmp);
		return (1);
	}
	item = tmp->next;
	free(tmp);
	tmp = NULL;
	if (tokenization_helper(item, minishell, tmp) == 1)
		return (1);
	return (0);
}
