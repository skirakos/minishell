/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_check_file.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skirakos <skirakos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 19:30:30 by skirakos          #+#    #+#             */
/*   Updated: 2024/11/20 14:10:01 by skirakos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_redir(char *str)
{
	if (ft_strcmp(str, "<") == 0
		|| ft_strcmp(str, ">") == 0
		|| ft_strcmp(str, "<<") == 0
		|| ft_strcmp(str, ">>") == 0)
		return (1);
	return (0);
}

int	open_file(char *fn, int type, t_minishell *minishell, int fd)
{
	if (type == IN_REDIR)
	{
		fd = open(fn, O_RDONLY);
		if (fd < 0)
			fd = -2;
	}
	else if (type == OUT_REDIR)
		fd = open(fn, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	else if (type == APPEND_REDIR)
		fd = open(fn, O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (fd > 2)
		return (fd);
	if (fd < 0)
	{
		if (fd == -1)
			print_err(1, "minihell: ", fn, ": Permission denied\n");
		if (fd == -2 && type == IN_REDIR)
			print_err(1, "minihell: ", fn, ": No such file or directory\n");
		minishell->file_err = 1;
	}
	if (type == IN_REDIR)
		return (0);
	return (1);
}

int	check_folder(char *str)
{
	struct stat	file_stat;

	if (stat(str, &file_stat) == 0 && S_ISDIR(file_stat.st_mode))
		return (1);
	return (0);
}

int	check_accsess(char *str)
{
	struct stat	file_stat;

	if (stat(str, &file_stat) == 0)
		return (1);
	return (0);
}

void	access_failed(t_minishell *minishell)
{
	if (check_accsess(minishell->cmd[0]) && access(minishell->cmd[0], X_OK) != 0
		&& access(minishell->cmd[0], W_OK) != 0)
	{
		print_err(126, "minishell: ", minishell->cmd[0],
			": Permission denied\n");
		exit (126);
	}
	if (check_folder(minishell->cmd[0])
		&& ((minishell->cmd[0][0] == '.' || minishell->cmd[0][0] == '/')
		|| (minishell->cmd[0][ft_strlen(minishell->cmd[0]) - 1] == '/')))
	{
		print_err(126, "minishell: ", minishell->cmd[0],
			": is a directory\n");
		exit (126);
	}
	perror_exit(minishell, CMD_NOT_FOUND, minishell->cmd[0], 1);
	exit (127);
}
