/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artyavet <artyavet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 20:42:01 by artyavet          #+#    #+#             */
/*   Updated: 2024/11/20 23:15:49 by artyavet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	init_pipe_fd(t_minishell *minishell, int pipe_count)
{
	int	(*fd)[2];
	int	i;

	i = 0;
	fd = malloc(pipe_count * sizeof(int [2]));
	if (!fd)
		return (0);
	while (i < pipe_count)
	{
		if (pipe(fd[i]) == -1)
		{
			while (i > 0)
			{
				close(fd[i - 1][0]);
				close(fd[i - 1][1]);
				i--;
			}
			free(fd);
		}
		i++;
	}
	minishell->fd = fd;
	return (1);
}

void	close_fd(t_minishell *minishell, int pipe_count)
{
	int	i;

	i = 0;
	while (i < pipe_count)
	{
		close(minishell->fd[i][0]);
		close(minishell->fd[i][1]);
		i++;
	}
}

void	ft_dups(t_minishell *minishell, int pipes, int curr)
{
	if (curr != 0 && minishell->fd_in == 0
		&& dup2(minishell->fd[curr - 1][0], 0) == -1)
	{
		close_fd(minishell, pipes);
		print_err(1, "minishell: ", "pipe error\n", NULL);
		exit(1);
	}
	if (curr < pipes
		&& dup2(minishell->fd[curr][1], 1) == -1)
	{
		close_fd(minishell, pipes);
		print_err(1, "minishell: ", "pipe error\n", NULL);
		exit(1);
	}
	close_fd(minishell, pipes);
}
