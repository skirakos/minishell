/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skirakos <skirakos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 13:55:52 by skirakos          #+#    #+#             */
/*   Updated: 2024/11/20 13:56:30 by skirakos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	here_doc_write(int fd, char *limiter)
{
	char	*input;

	input = NULL;
	while (1)
	{
		input = readline("");
		if (ft_strcmp(input, limiter) == 0)
		{
			free(input);
			break;
		}
		write(fd, input, ft_strlen(input));
		write(fd, "\n", 1);
		free(input);
	}
	close(fd);
}

int	here_doc_open(char *limiter)
{
	int	fd;

	fd = open(".heredocfile", O_WRONLY | O_CREAT | O_TRUNC, 0644); //hidden file starts with .
	if (fd == -1)
		return (-1);
	here_doc_write(fd, limiter);
	fd = open(".heredocfile", O_RDONLY);
	if (fd == -1)
		return (-1);
	return (fd);
}
