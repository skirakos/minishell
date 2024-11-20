/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization_quote_1.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artyavet <artyavet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 22:56:49 by artyavet          #+#    #+#             */
/*   Updated: 2024/11/20 22:57:26 by artyavet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	create_item_next(t_split **item, t_vars *vars)
{
	(*item)->next = malloc(sizeof(t_split));
	if (!(*item)->next)
		return (1);
	ft_strcut((*item)->next, vars->input, vars->start, vars->end);
	(*item) = (*item)->next;
	(*item)->next = NULL;
	return (0);
}

int	find_closing_quote(t_split **item, int *i, t_vars *vars)
{
	while (vars->input[*i] && (vars->input[*i] == ' '
			|| vars->input[*i] == '\t' || vars->input[*i] == '\n'))
		(*i)++;
	if ((vars->input[*i] == '|' || vars->input[*i] == '<'
			|| vars->input[*i] == '>'))
	{
		if (type_operator(item, vars->input, i))
			return (-1);
		(*i)++;
		vars->start = *i;
		return (1);
	}
	if (*i == 0 || (*i > 0 && (vars->input[*i - 1] == ' '
				|| vars->input[*i - 1] == '|' || vars->input[*i - 1] == '>'
				|| vars->input[*i - 1] == '<')))
		vars->start = *i;
	while (vars->input[*i] && !(vars->input[*i] == '"' || vars->input[*i] == 39)
		&& vars->input[*i] != '|' && vars->input[*i] != '<'
		&& vars->input[*i] != '>' && ((vars->input[*i] != ' '
				&& vars->input[*i] != '\t' && vars->input[*i] != '\n')))
		(*i)++;
	return (0);
}

int	check_input(t_vars *vars, int *i)
{
	if (vars->input[*i] == vars->current_quote
		&& (vars->input[*i + 1] == '\0'
			|| vars->input[*i + 1] == ' '
			|| vars->input[*i + 1] == '|'
			|| vars->input[*i + 1] == '<'
			|| vars->input[*i + 1] == '>'))
		return (1);
	return (0);
}

int	find_opening_quote(t_minishell *minishell, t_split **item,
	int *i, t_vars *vars)
{
	if (vars->input[*i] == '"' || vars->input[*i] == 39)
	{
		if (*i == 0 || (*i > 0 && vars->input[*i - 1] == ' '))
			vars->start = *i;
		vars->current_quote = vars->input[*i];
		(*i)++;
		if (quote_check(minishell, i, vars) == 1)
			return (-1);
		if (check_input(vars, i))
		{
			vars->end = *i;
			if (create_item_next(item, vars) == 1)
				return (-1);
			(*i)++;
			vars->start = ++vars->end;
			vars->current_quote = 0;
			return (1);
		}
		else if (vars->input[*i + 1] && (vars->input[*i + 1] != ' '))
		{
			(*i)++;
			return (1);
		}
	}
	return (0);
}
