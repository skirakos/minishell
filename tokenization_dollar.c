/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization_dollar.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artyavet <artyavet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 22:58:54 by artyavet          #+#    #+#             */
/*   Updated: 2024/11/20 22:59:12 by artyavet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*sedastan(char *str, int i, t_env *env, int end)
{
	char	*key;
	char	*before_key;
	char	*after_key;
	char	*dollar_value;
	int		start;

	if (str[i + 1] == '\0' || str [i + 1] == ' ')
		return (str);
	before_key = ft_substr(str, 0, i);
	start = ++i;
	if (str[i] && str[i] == '?')
		return (merge(before_key, ft_itoa(g_exit_status),
				ft_substr(str, i + 1, ft_strlen(str))));
	while (check_str(str, i))
		i++;
	end = i - 1;
	key = ft_substr(str, start, end + 1);
	after_key = ft_substr(str, end + 1, ft_strlen(str));
	dollar_value = get_from_env(env, key);
	free(str);
	free(key);
	return (merge(before_key, dollar_value, after_key));
}

void	dollar_var_init(t_dollar_var *var)
{
	var->str = NULL;
	var->i = 0;
	var->is_dquote = 0;
	var->is_squote = 0;
}

void	sign_helper(t_dollar_var *v, char *str)
{
	if (v->is_squote == 0 && str[v->i] == '\"')
		v->is_dquote = !v->is_dquote;
	if (v->is_dquote == 0 && str[v->i] == '\'')
		v->is_squote = !v->is_squote;
}

char	*dollar_sign_helper(char *str, t_dollar_var v,
	t_split *prev, t_env *env)
{
	while (str && str[v.i] != '\0')
	{
		sign_helper(&v, str);
		if (v.is_squote == 0 && str[v.i] == '$')
		{
			if (str[v.i + 1] == '\0' || (v.is_dquote && str[v.i + 1] == '"'))
			{
				free(str);
				str = ft_strdup("$");
				return (str);
			}
			else if (!prev || (prev && prev->type != HERE_DOC))
			{
				str = sedastan(str, v.i, env, 0);
				if (!str)
					return (NULL);
				if (str[v.i] == '$' && (str[v.i + 1] == '\0'
						|| str[v.i + 1] == ' '))
					v.i++;
				continue ;
			}
		}
		v.i++;
	}
	return (str);
}

int	dollar_sign(t_split *item, t_env *env)
{
	t_dollar_var	v;
	t_split			*tmp;
	t_split			*prev;
	char			*str;

	prev = NULL;
	tmp = item;
	dollar_var_init(&v);
	while (item && item->value)
	{
		v.i = 0;
		str = item->value;
		str = dollar_sign_helper(str, v, prev, env);
		if (!str)
			return (1);
		item->value = str;
		prev = item;
		item = item->next;
	}
	item = tmp;
	if (quote_remover(item))
		return (1);
	return (0);
}
