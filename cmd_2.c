/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skirakos <skirakos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 19:26:08 by skirakos          #+#    #+#             */
/*   Updated: 2024/11/20 14:04:48 by skirakos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_cmd_helper_2(t_minishell *minishell, char **path,
	t_env *tmp, int i)
{
	char	*joined_path;
	char	*tmp_join;

	while (path && path[i])
	{
		if (!is_builtin(minishell->cmd[0]))
		{
			joined_path = ft_strjoin(path[i], "/");
			tmp_join = joined_path;
			joined_path = ft_strjoin(joined_path, minishell->cmd[0]);
			free(tmp_join);
			if (access(joined_path, X_OK) == 0)
			{
				free(minishell->cmd[0]);
				minishell->cmd[0] = ft_strdup(joined_path);
				free(joined_path);
				free_matrix(path, matrix_len(path));
				minishell->env = tmp;
				return (0);
			}
			free(joined_path);
		}
		i++;
	}
	return (1);
}

int	check_cmd_helper_1(t_minishell *minishell)
{
	if ((minishell->cmd[0][0] == '.'
		&& minishell->cmd[0][1] == '/')
		|| search_slash(minishell->cmd[0]) == 0)
	{
		if (access(minishell->cmd[0], X_OK) == 0)
			return (-1);
		else if (minishell->cmd[0][0] == '.' && minishell->cmd[0][1] == '/')
			return (-1);
	}
	if (minishell->cmd[0][0] == '.' && minishell->cmd[0][1] == '/')
	{
		if (access(minishell->cmd[0], X_OK) != 0)
			return (-1);
	}
	return (0);
}
