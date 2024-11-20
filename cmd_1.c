/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skirakos <skirakos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 19:24:28 by skirakos          #+#    #+#             */
/*   Updated: 2024/11/20 14:03:50 by skirakos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	commands_helper(t_minishell *minishell, char **envp)
{
	if (ft_strcmp(minishell->cmd[0], "export") == 0)
	{
		export_bulki(minishell, envp);
		exit(0);
	}
	else if (ft_strcmp(minishell->cmd[0], "echo") == 0)
	{
		echo(minishell->cmd);
		exit(0);
	}
	else if (ft_strcmp(minishell->cmd[0], "pwd") == 0)
	{
		pwd();
		exit(0);
	}
	else if (ft_strcmp(minishell->cmd[0], "env") == 0)
	{
		env(minishell);
		exit(0);
	}
	else if (execve(minishell->cmd[0], minishell->cmd, envp) == -1)
	{
		perror_exit(minishell, EXECVE_ERR, minishell->cmd[0], 1);
		exit(1);
	}
}

void	commands(t_minishell *minishell, char **envp)
{
	if (minishell->file_err || !minishell->cmd[0])
		exit (1);
	if ((access(minishell->cmd[0], X_OK) != 0
			&& !is_builtin(minishell->cmd[0]))
		|| check_folder(minishell->cmd[0]))
		access_failed(minishell);
	else if (ft_strcmp(minishell->cmd[0], "cd") == 0)
	{
		cd(minishell, NULL);
		exit(0);
	}
	else if (ft_strcmp(minishell->cmd[0], "env") == 0)
	{
		env(minishell);
		exit(0);
	}
	else if (ft_strcmp(minishell->cmd[0], "unset") == 0)
	{
		unset(minishell);
		exit(0);
	}
	else if (ft_strcmp(minishell->cmd[0], "exit") == 0)
		exit_shell(minishell);
	else
		commands_helper(minishell, envp);
}

char	**clean_cmd(char **cmd)
{
	int	i;

	i = 0;
	while (cmd[i])
	{
		if (ft_strcmp(cmd[i], ">") == 0
			|| ft_strcmp(cmd[i], ">>") == 0
			|| ft_strcmp(cmd[i], "<") == 0
			|| ft_strcmp(cmd[i], "<<") == 0)
		{
			free(cmd[i]);
			if (cmd[i + 1])
				free(cmd[i++]);
			continue ;
		}
		i++;
	}
	return (cmd);
}

void	check_cmd(t_minishell *minishell)
{
	t_env	*tmp;
	char	**path;

	if (!minishell->cmd[0])
		return ;
	tmp = minishell->env;
	if (check_cmd_helper_1(minishell) == -1)
		return ;
	path = NULL;
	while (minishell->env)
	{
		if (ft_strcmp(minishell->env->var, "PATH") == 0)
			path = ft_split(minishell->env->value, ':');
		minishell->env = minishell->env->next;
	}
	if (!check_cmd_helper_2(minishell, path, tmp, 0))
		return ;
	minishell->env = tmp;
	if (path)
		free_matrix(path, matrix_len(path));
}
