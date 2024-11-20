/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artyavet <artyavet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 18:33:48 by artyavet          #+#    #+#             */
/*   Updated: 2024/11/17 18:33:50 by artyavet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid(char	*status)
{
	int	i;

	i = 0;
	if (status[0] == '+' || status[0] == '-')
		i++;
	while (status[i])
	{
		if (status[i] >= '0' && status[i] <= '9')
			i++;
		else
			return (-1);
	}
	return (0);
}

void	exit_shell(t_minishell *minihell)
{
	if (minihell->cmd[1] && ft_atoi(minihell->cmd[1]) == -1)
	{
		ft_putstr_fd("exit\n", 2);
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(minihell->cmd[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		g_exit_status = 255;
	}
	else if (matrix_len(minihell->cmd) > 2)
	{
		ft_putstr_fd("exit\n", 2);
		ft_putstr_fd("Minishell gjuk: exit: too many arguments\n", 2);
		g_exit_status = 1;
	}
	else if (minihell->cmd[1] && is_valid(minihell->cmd[1]) == 0)
	{
		ft_putstr_fd("exit\n", 2);
		g_exit_status = ft_atoi(minihell->cmd[1]);
		if (g_exit_status < 0 || g_exit_status > 255)
			g_exit_status = g_exit_status % 256;
	}
	free_before_exit(minihell);
	exit(g_exit_status);
}
