/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artyavet <artyavet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 18:37:38 by artyavet          #+#    #+#             */
/*   Updated: 2024/11/20 23:23:39 by artyavet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_exit_status = 0;

t_minishell	*minishell_init(char **env)
{
	t_minishell	*list;

	list = malloc(sizeof(t_minishell));
	if (!list)
		return (NULL);
	list->fd = NULL;
	list->tokens = NULL;
	list->cmd = NULL;
	list->pid = NULL;
	list->env = env_copy(env);
	if (!list->env)
	{
		free (list);
		return (NULL);
	}
	list->fd_in = 0;
	list->fd_out = 1;
	list->fd_heredoc = 0;
	return (list);
}

void	minishell_loop(t_minishell *minishell)
{
	char	*input;

	input = "";
	while (input)
	{
		set_sig_before_rl();
		input = readline("\033[1;90m⚙️  MINISHELL GJUK\033[0m: ");
		set_sig_after_rl();
		if (input && *input)
		{
			add_history(input);
			if (tokenization(input, minishell))
			{
				free(input);
				break ;
			}
		}
		free(input);
	}
}

int	main(int argc, char **argv, char **env)
{
	char		*input;
	t_minishell	*minishell;

	(void)argv;
	input = "";
	if (argc != 1)
		return (1);
	minishell = minishell_init(env);
	if (!minishell)
		return (1);
	minishell_loop(minishell);
	free_before_exit(minishell);
	ft_putstr_fd("exit\n", 1);
	return (0);
}
