/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skirakos <skirakos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 21:32:15 by artyavet          #+#    #+#             */
/*   Updated: 2024/11/19 17:57:28 by skirakos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	export_print_helper(char **envp, int i, int j, int flag)
{
	while (envp[i][j])
	{
		if (envp[i][j] == '=')
		{
			if (!envp[i][j + 1] && flag == 0)
				break ;
			else if (envp[i][j + 1] != '"' && flag == 0)
			{
				ft_putstr_fd("=\"", 1);
				flag = 1;
				j++;
			}
			else if (envp[i][j + 1] == '"')
			{
				ft_putstr_fd("=", 1);
				j++;
			}
		}
		write(1, &envp[i][j], 1);
		j++;
	}
	if (flag == 1)
		ft_putstr_fd("\"", 1);
}

char	**str_divide_by_equal_sign(char *str)
{
	int		i;
	char	**res;

	i = 0;
	res = (char **)malloc(3 * sizeof(char *));
	if (!res)
		return (NULL);
	while (str[i] && str[i] != '=')
		i++;
	res[0] = malloc(i + 1);
	res[1] = malloc(ft_strlen(str) - i);
	if (!res[0] || !res[1])
	{
		free(res[0]);
		free(res[1]);
		free(res);
		return (NULL);
	}
	for_copy(str, res);
	return (res);
}

void	exec_export(t_minishell *minishell, int i)
{
	char	**split;
	char	*new_value;

	new_value = NULL;
	while (minishell->cmd[i])
	{
		if (check_underscore(minishell->cmd[i], &i))
			continue ;
		if (minishell->cmd[i][0] == '=' || is_digit(minishell->cmd[i][0]))
		{
			print_err(1, "minishell: export: `", minishell->cmd[i++],
				"': not a valid identifier\n");
			continue ;
		}
		split = str_divide_by_equal_sign(minishell->cmd[i]);
		if (is_valid_var(split[0]))
		{
			print_err(1, "minishell: export: `", minishell->cmd[i++],
				"': not a valid identifier\n");
			continue ;
		}
		export_helper(minishell, split, new_value, i);
		free_matrix(split, matrix_len(split));
		i++;
	}
}

void	print_env(char **envp)
{
	int	i;
	int	j;
	int	flag;

	i = 0;
	while (envp[i])
	{
		j = 0;
		flag = 0;
		ft_putstr_fd("declare -x ", 1);
		export_print_helper(envp, i, j, flag);
		ft_putstr_fd("\n", 1);
		i++;
	}
}

void	export_bulki(t_minishell *minishell, char **envp)
{
	int	i;

	if (matrix_len(minishell->cmd) == 1)
	{
		merge_sort(envp, 0, matrix_len(envp) - 1);
		i = 0;
		print_env(envp);
	}
	else if (matrix_len(minishell->cmd) > 1)
	{
		exec_export(minishell, 1);
	}
}
