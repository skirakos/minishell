/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization_remove.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artyavet <artyavet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 22:59:51 by artyavet          #+#    #+#             */
/*   Updated: 2024/11/20 23:00:06 by artyavet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	remove_empty_nodes_continue(t_split **current,
	t_split **prev, t_split **head)
{
	*head = *current;
	*prev = *current;
	*current = (*current)->next;
	while (*current != NULL)
	{
		if ((*current)->value == NULL || (*current)->value[0] == '\0')
		{
			(*prev)->next = (*current)->next;
			free((*current)->value);
			free(*current);
			*current = (*prev)->next;
		}
		else
		{
			*prev = *current;
			*current = (*current)->next;
		}
	}
}

t_split	*remove_empty_nodes(t_split *item)
{
	t_split	*current;
	t_split	*prev;
	t_split	*head;
	t_split	*temp;

	current = item;
	prev = NULL;
	head = item;
	while (current != NULL
		&& (current->value == NULL || current->value[0] == '\0'))
	{
		temp = current;
		current = current->next;
		free(temp->value);
		free(temp);
	}
	if (current == NULL)
		return (NULL);
	remove_empty_nodes_continue(&current, &prev, &head);
	return (head);
}
