/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skirakos <skirakos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 17:39:40 by artyavet          #+#    #+#             */
/*   Updated: 2024/11/19 19:31:37 by skirakos         ###   ########.fr       */
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
			&& (minishell->tokens->type == WORD || minishell->tokens->type != S_PIPE))
	{
		if (minishell->tokens->type != WORD && check_redir(minishell->tokens->value))
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

void	initialization(t_minishell *minishell)
{
	minishell->pipes = pipe_count(minishell);
	minishell->pid = malloc(sizeof(pid_t) * (minishell->pipes + 1));
	if (!minishell->pid)
		free_before_exit(minishell);
	init_pipe_fd(minishell, minishell->pipes);
	minishell->is_builtin = 0;
}
void	built_in(t_minishell *minishell, int curr)
{
	t_split	*tmp;
	int		pid;
	char	**envp;

	tmp = minishell->tokens;
	while (minishell->tokens && minishell->tokens->value)
	{
		builtin_helper(minishell);
		envp = env_to_matrix(minishell);
		if (!envp)
			break ;
		if (++curr < minishell->pipes + 1)
		{
			if (minishell->pipes == 0 && isbuiltin(minishell->cmd[0]) == 1)
				pipes_zero(minishell, envp);
			else
			{
				pid = fork();
				fork_helper(minishell, envp, curr, pid);
			}
		}
		minishell->cmd = free_matrix(minishell->cmd, matrix_len(minishell->cmd));
		envp = free_matrix(envp, matrix_len(envp));
	}
	end_of_fork(minishell, tmp, pid);
}
