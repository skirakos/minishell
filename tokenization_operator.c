/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization_operator.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artyavet <artyavet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 22:53:14 by artyavet          #+#    #+#             */
/*   Updated: 2024/11/20 22:57:43 by artyavet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_operation(t_split **item)
{
	t_split	*t;

	(*item) = remove_empty_nodes((*item));
	if ((*item) && (*item)->type == S_PIPE)
		return (1);
	t = *item;
	while (t && t->value)
	{
		if (t->next)
		{
			if (t->next->type == S_PIPE && (t->type != WORD
					|| (t->next->next && t->next->next->type != WORD)))
				return (1);
			else if ((t->type == HERE_DOC || t->type == IN_REDIR
					|| t->type == OUT_REDIR || t->type == APPEND_REDIR)
				&& (t->next == NULL || t->next->type != WORD))
				return (1);
		}
		t = t->next;
	}
	return (0);
}

int	type_operator_helper_2(t_split **item, char *input, int *i)
{
	if (input[*i + 1] == '>')
	{
		(*item)->value = ft_strdup(">>");
		if (!(*item)->value)
			return (1);
		(*item)->type = APPEND_REDIR;
		(*i)++;
	}
	else
	{
		(*item)->value = ft_strdup(">");
		if (!(*item)->value)
			return (1);
		(*item)->type = OUT_REDIR;
	}
	return (0);
}

int	type_operator_helper(t_split **item, char *input, int *i)
{
	if (input[*i + 1] == '<')
	{
		(*item)->value = ft_strdup("<<");
		if (!(*item)->value)
			return (1);
		(*item)->type = HERE_DOC;
		(*i)++;
	}
	else
	{
		(*item)->value = ft_strdup("<");
		if (!(*item)->value)
			return (1);
		(*item)->type = IN_REDIR;
	}
	return (0);
}

int	type_operator(t_split **item, char *input, int *i)
{
	(*item)->next = malloc(sizeof(t_split));
	if (!(*item)->next)
		return (1);
	(*item) = (*item)->next;
	(*item)->next = NULL;
	if (input[*i] == '|')
	{
		(*item)->value = ft_strdup("|");
		if (!(*item)->value)
			return (1);
		(*item)->type = S_PIPE;
	}
	else if (input[*i] == '<')
	{
		if (type_operator_helper(item, input, i))
			return (1);
	}
	else if (input[*i] == '>')
	{
		if (type_operator_helper_2(item, input, i))
			return (1);
	}
	return (0);
}
