/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artyavet <artyavet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 19:20:30 by skirakos          #+#    #+#             */
/*   Updated: 2024/11/20 23:13:54 by artyavet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	create_envp(t_minishell *minishell, char **envp)
{
	int		i;
	char	*join_tmp;
	char	*zibil;

	i = 0;
	while (minishell && minishell->env)
	{
		join_tmp = ft_strjoin(minishell->env->var, "=");
		zibil = join_tmp;
		join_tmp = ft_strjoin(join_tmp, minishell->env->value);
		free(zibil);
		envp[i] = ft_strdup(join_tmp);
		free(join_tmp);
		join_tmp = NULL;
		i++;
		minishell->env = minishell->env->next;
	}
	envp[i] = NULL;
}

char	**env_to_matrix(t_minishell *minishell)
{
	int		len;
	t_env	*env;
	char	**envp;

	env = minishell->env;
	len = 0;
	while (minishell && minishell->env)
	{
		len++;
		minishell->env = minishell->env->next;
	}
	minishell->env = env;
	envp = (char **)malloc(sizeof(char *) * (len + 1));
	if (!envp)
		return (NULL);
	create_envp(minishell, envp);
	minishell->env = env;
	return (envp);
}

void	handle_redirection(t_minishell *minishell, int type, char *file_name)
{
	if (minishell->file_err)
		return ;
	if (type == IN_REDIR)
		minishell->fd_in = open_file(file_name, IN_REDIR, minishell, -1);
	else if (type == APPEND_REDIR)
		minishell->fd_out = open_file(file_name, APPEND_REDIR, minishell, -1);
	else if (type == OUT_REDIR)
		minishell->fd_out = open_file(file_name, OUT_REDIR, minishell, -1);
	else if (type == HERE_DOC)
	{
		minishell->fd_heredoc = here_doc_open(file_name);
		return ;
	}
}

void	create_matrix(t_minishell *minishell, t_split **tmp, char **matrix)
{
	int	i;

	i = 0;
	while ((*tmp) && (*tmp)->value && (*tmp)->type != S_PIPE)
	{
		if ((*tmp)->type == WORD || !check_redir((*tmp)->value))
		{
			matrix[i] = ft_strdup((*tmp)->value);
			i++;
		}
		else
		{
			handle_redirection(minishell, (*tmp)->type, (*tmp)->next->value);
			(*tmp) = (*tmp)->next;
		}
		(*tmp) = (*tmp)->next;
	}
}
