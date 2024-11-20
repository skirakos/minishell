/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skirakos <skirakos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 19:27:14 by skirakos          #+#    #+#             */
/*   Updated: 2024/11/20 14:07:35 by skirakos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	fork_helper(t_minishell *minishell, char **envp,
			int curr, int pid)
{
	if (pid == -1 && g_exit_status != 1)
		perror_exit(minishell, FORK_ERR, "Resource temporarily unavailable", 1);
	else if (pid == 0)
	{
		minishell->is_builtin = 0;
		ft_dups(minishell, minishell->pipes, curr);
		redirs(minishell);
		commands(minishell, envp);
	}
	else
		minishell->pid[curr] = pid;
}

void	wait_processes(t_minishell *minishell, int pipes)
{
	int		i;
	int		exit_status;

	i = 0;
	if ((pipes == 0 && minishell->is_builtin) || minishell->file_err)
		return ;
	while (i < pipes + 1)
	{
		waitpid(minishell->pid[i], &exit_status, 0);
		if (WIFEXITED(exit_status))
			g_exit_status = WEXITSTATUS(exit_status);
		else if (WIFSIGNALED(exit_status))
			g_exit_status = 128 + WTERMSIG(exit_status);
		i++;
	}
}

int	pipe_count(t_minishell *minishell)
{
	int		pipe_count;
	t_split	*tmp;

	tmp = minishell->tokens;
	pipe_count = 0;
	if (!minishell->tokens || !minishell->tokens->value)
	{
		ft_putstr_fd("return \n", 1);
		return (0);
	}
	while (minishell && minishell->tokens)
	{
		if (minishell->tokens->type == S_PIPE)
			pipe_count++;
		minishell->tokens = minishell->tokens->next;
	}
	minishell->tokens = tmp;
	return (pipe_count);
}

void	pipes_zero(t_minishell *minishell, char **envp)
{
	minishell->is_builtin = 1;
	if (ft_strcmp(minishell->cmd[0], "cd") == 0)
		cd(minishell, NULL);
	else if (ft_strcmp(minishell->cmd[0], "env") == 0)
		env(minishell);
	else if (ft_strcmp(minishell->cmd[0], "unset") == 0)
		unset(minishell);
	else if (ft_strcmp(minishell->cmd[0], "exit") == 0)
		exit_shell(minishell);
	else if (ft_strcmp(minishell->cmd[0], "export") == 0)
		export_bulki(minishell, envp);
}

void	end_of_fork(t_minishell *minishell, t_split *tmp, int pid)
{
	close_fd(minishell, minishell->pipes);
	wait_processes(minishell, minishell->pipes);
	if (pid == -1)
		g_exit_status = 1;
	if (minishell->fd_in > 0)
		close(minishell->fd_in);
	minishell->fd_in = 0;
	if (minishell->fd_out > 1)
		close(minishell->fd_out);
	minishell->fd_out = 1;
	minishell->tokens = tmp;
	while (minishell->tokens)
	{
		tmp = minishell->tokens;
		free (minishell->tokens->value);
		minishell->tokens = minishell->tokens->next;
		free(tmp);
	}
	free(minishell->pid);
	minishell->pid = NULL;
	free(minishell->fd);
	minishell->fd = NULL;
}
