/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artyavet <artyavet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 17:39:40 by artyavet          #+#    #+#             */
/*   Updated: 2024/11/20 23:25:14 by artyavet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_split	*pre_execve(t_minishell *minishell)
{
	t_split	*tmp;
	int		len;
	char	**matrix;

	tmp = minishell->tokens;
	len = 0;
	while (minishell->tokens && minishell->tokens->value
		&& (minishell->tokens->type == WORD
			|| minishell->tokens->type != S_PIPE))
	{
		if (minishell->tokens->type != WORD
			&& check_redir(minishell->tokens->value))
			len -= 2;
		len++;
		minishell->tokens = minishell->tokens->next;
	}
	matrix = (char **)malloc(sizeof(char *) * (len + 1));
	if (!matrix)
		return (NULL);
	create_matrix(minishell, &tmp, matrix);
	matrix[len] = NULL;
	minishell->cmd = matrix;
	if (tmp && tmp->type == S_PIPE)
		minishell->tokens = minishell->tokens->next;
	return (minishell->tokens);
}

void	builtin_helper(t_minishell *minishell)
{
	minishell->file_err = 0;
	if (minishell->fd_in > 0)
		close(minishell->fd_in);
	minishell->fd_in = 0;
	if (minishell->fd_out > 1)
		close(minishell->fd_out);
	minishell->fd_out = 1;
	minishell->tokens = pre_execve(minishell);
	check_cmd(minishell);
	if (!minishell->cmd[0])
		minishell->is_builtin = 1;
}

int	initialization(t_minishell *minishell)
{
	minishell->pipes = pipe_count(minishell);
	minishell->pid = malloc(sizeof(pid_t) * (minishell->pipes + 1));
	if (!minishell->pid)
	{
		free_before_exit(minishell);
		return (1);
	}
	init_pipe_fd(minishell, minishell->pipes);
	minishell->is_builtin = 0;
	return (0);
}

void	built_in(t_minishell *mini, int curr)
{
	t_split	*tmp;
	int		pid;
	char	**envp;

	tmp = mini->tokens;
	while (mini->tokens && mini->tokens->value)
	{
		builtin_helper(mini);
		envp = env_to_matrix(mini);
		if (!envp)
			break ;
		if (++curr < mini->pipes + 1)
		{
			if (mini->pipes == 0 && isbuiltin(mini->cmd[0]) == 1)
				pipes_zero(mini, envp);
			else
			{
				pid = fork();
				fork_helper(mini, envp, curr, pid);
			}
		}
		mini->cmd = free_matrix(mini->cmd, matrix_len(mini->cmd));
		envp = free_matrix(envp, matrix_len(envp));
	}
	end_of_fork(mini, tmp, pid);
}
