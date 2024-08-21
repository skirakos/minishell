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
void	dollar_sign(t_split *item, t_env *env)
{
	int		i;
	int		flag;
	int		start;
	int		end;
	char	*key;
	char	*before_key;
	char	*after_key;
	char	*dollar_value;
	char	*new_value;
	t_split	*tmp;

	tmp = item;
	start = 0;
	end = 0;
	flag = 0;
	while (item && item->value)
	{
		i = 0;
		while (item->value[i])
		{
			printf("first i: %d\n", i);
			printf("value[i]: %c\n", item->value[i]);
			while (item->value[i] && (item->value[i] != '$' && item->value[i] != '\0'))
				i++;
			if (item->value[i] == '\0')
				break;
			if (item->value[i] && item->value[i] == '$')
			{
				flag = 1;
				i++;
				start = i;
				before_key = ft_substr(item->value, 0, start - 1);
				printf("before_key: %s\n", before_key);
				while (item->value[i] && (item->value[i] != ' ' && item->value[i] != '"' && item->value[i] != 39 && item->value[i] != '>' && item->value[i] != '<' && item->value[i] != '>' && item->value[i] != '\0'))
					i++;
				end = i - 1;
				printf("start:  %d ----- end:  %d\n", start, end);
				key = ft_substr(item->value, start, end + 1);
				printf("keeeeyyyy: %s\n", key);
				printf("i: %d\n", i);
				//printf("start:  %d ----- end:  %d\n", end + ft_strlen(key), ft_strlen(item->value) - 1);
				after_key = ft_substr(item->value, end + 1, ft_strlen(item->value));
				printf("after_key: %s\n", after_key);
				if (check_key_in_env(env, key, item) == 1)
				{
					
					printf("get_from_env\n");
					dollar_value = get_from_env(env, key);
					printf("dollar value: %s\n", dollar_value);
					printf("get_from_env\n");
					new_value = ft_strjoin(before_key, dollar_value);
					new_value = ft_strjoin(new_value, after_key);
				}
				else
				{
					new_value = ft_strjoin(before_key, after_key);
				}
			}
			// if (item->value[i] != '\0')
			if (flag == 1)
			{
				if (new_value[0] == '\0')
				{
					free(item->value);
					item->value = new_value;
					i = 0;
					continue ;
				}
				printf("asvzdv s\n");
				//printf("new value: %s\n", new_value);
				free(item->value);
				item->value = malloc(ft_strlen(new_value) + 1);
				if (!item->value)
					return ;
				int j = 0;
				printf("new value: %s\n", new_value);
				while (item->value[j] && new_value[j])
				{
					item->value[j] = new_value[j];
					j++;
				}
				item->value[j] = '\0';
				flag = 0;
			}
			printf("after first iteration: %s ---- %d\n", item->value, i);
			i++;
		}
		
		item = item->next;
	}
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
		if (input[i] == '$' && (i > 0 && (input[i - 1] != '"' && input[i - 1] != 39)))
		{
			start = i;
			printf("shit\n");
			i++;
			while (input[i] && (input[i] != ' ' || input[i] != '\0'))
			{
				i++;
			}
			end = i - 1;
			i--;
			item->next = malloc(sizeof(t_split));
			if (!item->next)
				return ;
			ft_strcut(item->next, input, start, end);
			start = end + 2;
			item = item->next;
			item->next = NULL;
			i++;
			continue ;
		}
		if (input[i] == '"' || input[i] == 39)
		{
			if (i == 0 || (i > 0 && input[i - 1] == ' '))
				start = i;
			current_quote = input[i];
			i++;
			while (input[i] && input[i] != current_quote)
				i++;
			if (input[i] != current_quote)
			{
				exit(1 && write(2, "Error1\n", 7));
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
			printf("aaaa\n");
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
