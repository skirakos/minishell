/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   merge_sort.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artyavet <artyavet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 23:14:00 by artyavet          #+#    #+#             */
/*   Updated: 2024/11/20 23:14:01 by artyavet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_merge_var(t_merge_var *var, int left, int right, int mid)
{
	var->n1 = mid - left + 1;
	var->n2 = right - mid;
	var->k = left;
	var->l = malloc(var -> n1 * sizeof(char *));
	if (!var->l)
		exit(1);
	var -> r = malloc(var -> n2 * sizeof(char *));
	if (!var->r)
		exit(1);
	var->i = 0;
	var->j = 0;
}

void	merge_sorted_arrays(char **arr, int left, int mid, int right)
{
	t_merge_var	var;

	init_merge_var(&var, left, right, mid);
	while (++var.i - 1 < var.n1)
		var.l[var.i - 1] = arr[left + var.i - 1];
	while (++var.j - 1 < var.n2)
		var.r[var.j - 1] = arr[mid + var.j];
	var.i = 0;
	var.j = 0;
	while (var.i < var.n1 && var.j < var.n2)
	{
		if (ft_strcmp(var.l[var.i], var.r[var.j]) <= 0)
			arr[var.k] = var.l[var.i++];
		else
			arr[var.k] = var.r[var.j++];
		var.k++;
	}
	while (var.i < var.n1)
		arr[var.k++] = var.l[var.i++];
	while (var.j < var.n2)
		arr[var.k++] = var.r[var.j++];
	free(var.l);
	free(var.r);
}

void	merge_sort(char **arr, int left, int right)
{
	int	mid;

	if (left < right)
	{
		mid = left + (right - left) / 2;
		merge_sort(arr, left, mid);
		merge_sort(arr, mid + 1, right);
		merge_sorted_arrays(arr, left, mid, right);
	}
}
