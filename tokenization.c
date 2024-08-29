#include "minishell.h"

char	*get_from_env(t_env *env, char *key)
{
	char	*res;

	while (env)
	{
		if (!ft_strncmp(env->var, key, ft_strlen(env->var)))
		{
			res = (char *)malloc(sizeof(char *) * (ft_strlen(env->value) + 1));
			if (!res)
				return (NULL);
			int j = 0;
			while (res[j] && env->value[j])
			{
				res[j] = env->value[j];
				j++;
			}
			res[j] = '\0';
			printf("res: %s\n", res);
			return (res);
		}
		env = env->next;
	}
	return (NULL);
}
void	quote_remover_continue(char *value, int start, int end)
{
	int	i;
	int	j;
	char	*str;

	str = malloc(ft_strlen(value) - 1);
	i = 0;
	j = 0;
	printf("quote_remover_continue\n");
	while (value[i])
	{
		if (i != start && i != end)
		{
			str[j] = value[j];
			j++;
		}
		i++;
	}
	str[j] = '\0';
	free(value);
	printf("quote_remover_str: %s\n", str);
	value = str;
}

void	quote_remover(t_split *item)
{
	int		i;
	int		startq;
	int		endq;
	char	curr;

	startq = 0;
	endq = 0;
	printf("quote_remover\n");
	while (item && item->value)
	{
		i = 0;
		while (item->value[i])
		{
			if (item->value[i] == '"' || item->value[i] == 39)
			{
				curr = item->value[i];
				startq = i;
				i++;
				while (item->value[i] && item->value[i] != curr)
					i++;
				endq = i;
				printf("endq: %d\n", endq);
				quote_remover_continue(item->value, startq, endq);
				continue ;
			}
			i++;
		}
		item = item->next;
	}
}

char	*merge(char *before_key, char *dollar_value, char *after_key)
{
	char	*tmp;
	char	*res;

	tmp = ft_strjoin(before_key, dollar_value);
	free(before_key);
	free(dollar_value);
	res = ft_strjoin(tmp, after_key);
	free(tmp);
	free(after_key);
	return (res);
}

char	*sedastan(char *str, int *i, t_env *env, int end)
{
	char	*key;
	char	*before_key;
	char	*after_key;
	char	*dollar_value;
	int		start;

	before_key = ft_substr(str, 0, *i);
	start = ++(*i);
	printf("before_key: %s\n", before_key);
	while (str[*i] && (str[*i] != ' ' && str[*i] != '/' && str[*i] != '$' && str[*i] != '"' && str[*i] != 39 && str[*i] != '>' && str[*i] != '<' && str[*i] != '>' && str[*i] != '\0'))
		(*i)++;
	end = (*i) - 1;
	printf("enddddd: %d\n", end);
	key = ft_substr(str, start, end + 1);
	printf("keeeeyyyy: %s\n", key);
	printf("i: %d\n", *i);
	after_key = ft_substr(str, end + 1, ft_strlen(str));
	printf("after_key: %s\n", after_key);
	dollar_value = get_from_env(env, key);
	return (merge(before_key, dollar_value, after_key));
}
void	dollar_sign(t_split *item, t_env *env)
{
	char	*str;
	int		i;
	int		is_dquote;
	int		is_squote;	
	
	i = 0;
	is_dquote = 0;
	is_squote = 0;
	while (item && item->value)
	{
		i = 0;
		str = item->value;
		while (str && str[i] != '\0')
		{
			if (is_squote == 0 && str[i] == '\"')
				is_dquote = !is_dquote;
			if (is_dquote == 0 && str[i] == '\'')
				is_squote = !is_squote;
			if (is_squote == 0 && str[i] == '$')
			{
				str = sedastan(str, &i, env, 0);
				continue ;
			}
			i++;
		}
		item->value = str;
		item = item->next;
	}
	quote_remover(item);
}

void	type_operator(t_split **item, char *input, int *i)
{
	(*item)->next = malloc(sizeof(t_split));
	if (!(*item)->next)
		return ;
	(*item) = (*item)->next;
	if (input[*i] == '|')
	{
		(*item)->value = "|";
		(*item)->type = "pipe";
	}
	else if (input[*i] == '<')
	{
		if (input[*i] && input[*i + 1] == '<')
		{
			(*item)->value = "<<";
			(*item)->type = "heredoc";
			(*i)++;
		}
		else
		{
			(*item)->value = "<";
			(*item)->type = "outredir";
		}
	}
	else if (input[*i] == '>')
	{
		if (input[*i] && input[*i + 1] == '>')
		{
			(*item)->value = ">>";
			(*item)->type = "append";
			(*i)++;
		}
		else
		{
			(*item)->value = ">";
			(*item)->type = "inredir";
		}
	}
	(*item)->next = NULL;
}

void	ft_strcut(t_split *item, char *input, int start, int end)
{
	int	i;

	i = 0;
	item->value = malloc(end - start + 2);
	if (!item->value)
		return ;
	while (item->value[i] && input[i] && start <= end)
	{
		item->value[i] = input[start];
		i++;
		start++;
	}
	item->value[i] = '\0';
	item->type = "word";
}

void	tokenization(char *input, t_env	*env)
{
	int		i;
	int		start;
	int		end;
	char	current_quote;
	t_split	*item;
	t_split	*tmp;

	item = ft_lstnew(NULL, NULL);
	if (!item)
		return ;
	tmp = item;
	i = 0;
	end = 0;
	start = 0;
	while (input[i])
	{
		if (input[i] == '"' || input[i] == 39)
		{
			if (i == 0 || (i > 0 && input[i - 1] == ' '))
				start = i;
			current_quote = input[i];
			i++;
			while (input[i] && input[i] != current_quote)
				i++;
			if (input[i] != current_quote && input[i] != '\0')
			{
				exit(1 && write(2, "Error4\n", 7));
			}
			if (input[i] == current_quote && (input[i + 1] == '\0' || input[i + 1] == ' ' || input[i + 1] == '|' || input[i + 1] == '<' || input[i + 1] == '>' || input[i + 1] == '$'))
			{
				if (input[i + 1] && input[i + 1] == '$')
				{
					i++;
					while (input[i] && input[i] != current_quote)
						i++;
				}
				end = i;
				item->next = malloc(sizeof(t_split));
				if (!item->next)
					return ;
				ft_strcut(item->next, input, start, end);
				start = end+2;
				item = item->next;
				item->next = NULL;
				i++;
				continue ;
				
			}
			else if (input[i + 1] && (input[i + 1] != ' '))
			{
				i++;
				continue ;
			}
		}
		while (input[i] && (input[i] == ' ' || input[i] == '\t' || input[i] == '\n'))
			i++;
		if ((input[i] == '|' || input[i] == '<' || input[i] == '>'))
		{
			type_operator(&item, input, &i);
			i++;
			start++;
			continue ;
		}
		if (i == 0 || (i > 0 && (input[i - 1] == ' ' || input[i - 1] == '|' || input[i - 1] == '>' || input[i - 1] == '<')))
			start = i;
		while (input[i] && !(input[i] == '"' || input[i] == 39) && input[i] != '|' && input[i] != '<' 
			&& input[i] != '>' && ((input[i] != ' ' && input[i] != '\t' && input[i] != '\n')))
			i++;
		if (input[i] == '"' || input[i] == 39)
		{
			current_quote = input[i];
			continue ;
		}
		end = i - 1;
		i--;
		item->next = malloc(sizeof(t_split));
		if (!item->next)
			return ;
		ft_strcut(item->next, input, start, end);
		item = item->next;
		item->next = NULL;
		i++;
	}
	item = tmp->next;
	free(tmp);
	tmp = item;
	dollar_sign(item, env);
	i = 0;
	while (tmp)
	{
		printf("%s\n", tmp->value);
		tmp = tmp->next;
	}
}

void	dollar(t_split	*item, t_env *env)
{
	int		i;
	char	*key;

	while (item)
	{
		int	start;
		i = 0;
		while (item->value[i])
		{
			if (item->value[i] == '$')
			{
				i++;
				start = i;
				while (item->value[i] && ((item->value[i] >= 'a' && item->value[i] <= 'z') || (item->value[i] >= 'A' && item->value[i] <= 'Z') || (item->value[i] >= '0' && item->value[i] <= '9') || (item->value[i] == '_')))
					i++;
				key = ft_substr(item->value, start, i - 1);
				if (check_key_in_env(env, key, item) == 1)
				{
					
				}
				else
				{
					free(item->value);
					item->value = NULL;
					break;
				}
			}
			else
				i++;
		}
		item = item->next;
	}
}

int	check_key_in_env(t_env *env, char *key, t_split *item)
{
	printf("key: %s\n", key);
	while (env)
	{
		if (!ft_strncmp(env->var, key, ft_strlen(env->var)))
		{
			printf("check_key_in_env: %s\n", item->value);
			int	i = 0;
			while (item->value[i] && env->value[i])
			{
				item->value[i] = env->value[i];
				i++;
			}
			printf("check_key_in_env: %s\n", item->value);
			return (1);
		}
		env = env->next;
	}
	printf("freee\n");
	free(item->value);
	item->value = NULL;
	printf("item->value: %s\n", item->value);
	printf("freee2\n");
	return (-1);
}
