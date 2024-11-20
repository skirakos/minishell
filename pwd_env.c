/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skirakos <skirakos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 13:41:43 by skirakos          #+#    #+#             */
/*   Updated: 2024/11/20 13:43:42 by skirakos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	env(t_minishell *minishell)
{
	t_env	*tmp;

	tmp = minishell->env;
	while (minishell->env)
	{
		if (minishell->env->value && ft_strcmp(minishell->env->value, "\"\"") != 0)
		{
			ft_putstr_fd(minishell->env->var, 1);
			ft_putstr_fd("=", 1);
			ft_putstr_fd(minishell->env->value, 1);
			ft_putstr_fd("\n", 1);
		}
		else if (minishell->env->value && ft_strcmp(minishell->env->value, "\"\"") == 0)
		{
			ft_putstr_fd(minishell->env->var, 1);
			ft_putstr_fd("=", 1);
			ft_putstr_fd("\n", 1);
		}
		minishell->env = minishell->env->next;
	}
	minishell->env = tmp;
}

void	pwd(void)
{
	char	pwd[PATH_MAX];

	if (!getcwd(pwd, PATH_MAX))
		return ;
	ft_putstr_fd(pwd, 1);
	ft_putstr_fd("\n", 1);
}
