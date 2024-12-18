/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artyavet <artyavet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 23:17:52 by artyavet          #+#    #+#             */
/*   Updated: 2024/11/20 23:18:13 by artyavet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	in_redir(t_minishell *minishell)
{
	if (minishell->fd_in > 0)
	{
		if (dup2(minishell->fd_in, 0) == -1)
		{
			close(minishell->fd_in);
			print_err(2, "minishell: ", "dup2 error\n", "");
		}
		close(minishell->fd_in);
	}
}

void	out_redir(t_minishell *minishell)
{
	if (minishell->fd_out > 1)
	{
		if (dup2(minishell->fd_out, 1) == -1)
		{
			close(minishell->fd_out);
			print_err(2, "minishell: ", "dup2 error\n", "");
		}
		close(minishell->fd_out);
	}
}

void	redirs(t_minishell *minishell)
{
	in_redir(minishell);
	out_redir(minishell);
}
