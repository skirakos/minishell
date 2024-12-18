/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artyavet <artyavet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 13:59:40 by skirakos          #+#    #+#             */
/*   Updated: 2024/11/20 23:19:41 by artyavet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_ctrl_op(t_split	*token)
{
	if (!token->value)
		return (1);
	if (token->type == S_PIPE || token->type == D_PIPE \
		|| token->type == S_AND || token->type == D_AND)
		return (1);
	return (0);
}

int	is_permited(t_split *token)
{
	if (token->type == D_PIPE
		|| token->type == S_AND
		|| token->type == D_AND)
		return (1);
	return (0);
}

void	print_err(int exit_status, char *msg1,
	char *msg2, char *msg3)
{
	g_exit_status = exit_status;
	if (msg1 != NULL)
		ft_putstr_fd(msg1, 2);
	if (msg2 != NULL)
		ft_putstr_fd(msg2, 2);
	if (msg3 != NULL)
		ft_putstr_fd(msg3, 2);
}

void	perror_exit(t_minishell *minishell, int err_code,
	char *msg, int fork)
{
	if (err_code == SYNTAX_ERR)
		print_err(2, "minishell: syntax error near unexpected token `",
			msg, "'\n");
	else if (err_code == FORK_ERR)
		print_err(1, "minishell: ", msg, "\n");
	else if (err_code == PIPE_ERR)
		print_err(2, "minishell: pipe error\n", msg, "\n");
	else if (err_code == QUOTE_ERR)
		print_err(2, "minishell: quote error \n", NULL, NULL);
	else if (err_code == CMD_NOT_FOUND)
		print_err(2, "minishell: ", msg, ": command not found\n");
	else if (err_code == EXECVE_ERR)
		print_err(2, "minishell: ", msg, ": execution failed\n");
	if (!fork)
		free_before_exit(minishell);
}

int	syntax_check(t_minishell *mini, t_split *tokens)
{
	if (tokens && tokens->type == S_PIPE)
		return (perror_exit(mini, SYNTAX_ERR, tokens->value, 1), 1);
	while (tokens != NULL)
	{
		if (tokens->type == OUT_REDIR || tokens->type == IN_REDIR \
			|| tokens->type == APPEND_REDIR || tokens->type == HERE_DOC)
		{
			if (tokens->next == NULL)
				return (perror_exit(mini, SYNTAX_ERR, "newline", 1), 1);
			else if (tokens->next->type == S_PIPE)
				return (perror_exit(mini, SYNTAX_ERR, tokens->value, 1), 1);
		}
		else if ((tokens->type == S_PIPE && !tokens->next)
			|| (tokens->type == S_PIPE && is_ctrl_op(tokens->next)))
			return (perror_exit(mini, SYNTAX_ERR, tokens->value, 1), 1);
		tokens = tokens->next;
	}
	return (0);
}
