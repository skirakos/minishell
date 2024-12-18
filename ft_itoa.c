/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artyavet <artyavet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 23:12:37 by artyavet          #+#    #+#             */
/*   Updated: 2024/11/20 23:12:38 by artyavet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	int	digit_count(int n)
{
	int	len;

	len = 0;
	if (n < 0)
		len++;
	while (n != 0)
	{
		n /= 10;
		len++;
	}
	return (len);
}

static char	*zerocase(char *m)
{
	m = (char *)malloc(sizeof(char) * 2);
	m[0] = 0 + '0';
	m[1] = 0;
	return (m);
}

char	*ft_itoa(int n)
{
	char	*p;
	int		len;
	long	val;

	p = NULL;
	if (n == 0)
		return (zerocase(p));
	len = digit_count(n);
	p = (char *)malloc(sizeof(char) * (len + 1));
	if (!p)
		return (NULL);
	val = (long)n;
	if (val < 0)
	{
		p[0] = '-';
		val *= -1;
	}
	p[len] = '\0';
	len--;
	while (val != 0)
	{
		p[len--] = (val % 10) + '0';
		val /= 10;
	}
	return (p);
}
