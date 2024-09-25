#include "minishell.h"

char	*get_from_env(t_env *env, char *key)
{
	char	*res;

	while (env)
	{
		if (!ft_strncmp(env->var, key, ft_strlen(env->var)))
		{
			printf("hey\n");
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
	printf("durs eka\n");
	return (NULL);
}
// char	*quote_remover_continue(char *value, int start, int end)
// {
// 	int	i;
// 	int	j;
// 	char	*str;

// 	str = malloc(ft_strlen(value) - 1);
// 	i = 0;
// 	j = 0;
// 	printf("quote_remover_continue\n");
// 	while (value[i])
// 	{
// 		if (i != start && i != end)
// 		{
// 			str[j] = value[i];
// 			j++;
// 		}
// 		i++;
// 	}
// 	str[j] = '\0';
// 	return (str);
// 	printf("quote_remover_str?????: %s\n", str);
// }

// void	quote_remover(t_split *item)
// {
// 	int		i;
// 	int		startq;
// 	int		endq;
// 	char	curr;

// 	startq = 0;
// 	endq = 0;
// 	printf("charcharchar: %s\n", item->value);
// 	while (item && item->value)
// 	{
// 		i = 0;
// 		while (item->value[i])
// 		{
// 			if (item->value[i] == '"' || item->value[i] == 39)
// 			{
// 				curr = item->value[i];
// 				startq = i;
// 				i++;
// 				while (item->value[i] && item->value[i] != curr)
// 					i++;
// 				endq = i;
// 				printf("endq: %d\n", endq);
// 				item->value = quote_remover_continue(item->value, startq, endq);
// 				continue ;
// 			}
// 			i++;
// 		}
// 		item = item->next;
// 	}
// 	printf("quote_remoooooover\n");
// }

void quote_remover(t_split *item) {
	int		len;
	int		i;
	int		j;
	int		in_single_quote;
	int		in_double_quote;
	char	*result;

    while (item && item->value) {
        len = ft_strlen(item->value);
        result = malloc(len + 1);
        if (!result)
            return;
        j = 0;
        in_single_quote = 0;
        in_double_quote = 0;
        i = 0;
        while (i < len) {
			if (item->value[i] == '\'' && !in_double_quote) {
                in_single_quote = !in_single_quote;
            } else if (item->value[i] == '"' && !in_single_quote) {
                in_double_quote = !in_double_quote;
            } else {
                result[j++] = item->value[i];
            }
            i++;
        }
        result[j] = '\0';
        printf("Original: %s\n", item->value);
        printf("Cleaned: %s\n", result);
        free(item->value);
        item->value = malloc(ft_strlen(result) + 1);
        if (item->value) {
            ft_strcpy(item->value, result);
        }
        free(result);
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
	printf("res-->%s$\n",res);
	return (res);
}

char	*sedastan(char *str, int i, t_env *env, int end)
{
	char	*key;
	char	*before_key;
	char	*after_key;
	char	*dollar_value;
	int		start;

	before_key = ft_substr(str, 0, i);
	start = ++(i);
	while (str[i] && (str[i] != ' ' && str[i] != '/' && str[i] != '$' && str[i] != '"' && str[i] != 39 && str[i] != '>' && str[i] != '<' && str[i] != '>' && str[i] != '\0'))
		(i)++;
	end = (i) - 1;
	//printf("enddddd: %d\n", end);
	key = ft_substr(str, start, end + 1);
	printf("keeeeyyyy: %s\n", key);
	//printf("i: %d\n", *i);
	after_key = ft_substr(str, end + 1, ft_strlen(str));
	dollar_value = get_from_env(env, key);
	printf("before_key: %s$\n", before_key);

	printf("after_key: %s$\n", after_key);
	return (merge(before_key, dollar_value, after_key));
}
void	dollar_sign(t_split *item, t_env *env)
{
	char	*str;
	int		i;
	int		is_dquote;
	int		is_squote;
	t_split	*tmp;
	t_split	*prev;
	
	i = 0;
	prev = NULL;
	tmp = item;
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
				if (!prev || (prev && prev->type != HERE_DOC))
				{
					str = sedastan(str, i, env, 0);
					continue ;
				}
			}
			i++;
		}
		//int j = 0;
		item->value = str;
		prev = item;
		item = item->next;
	}
	item = tmp;
	// if (prev)
	// {
	// 	free(prev->value);
	// 	free(prev);
	// }
	quote_remover(item);
	item = tmp;
}

void type_operator(t_split **item, char *input, int *i)
{
    (*item)->next = malloc(sizeof(t_split));
    if (!(*item)->next)
        return;

    (*item) = (*item)->next;
    (*item)->next = NULL; // Initialize next pointer

    switch (input[*i])
    {
        case '|':
            (*item)->value = ft_strdup("|");
            (*item)->type = S_PIPE;
            break;
        case '<':
            if (input[*i + 1] == '<')
            {
                (*item)->value = ft_strdup("<<");
                (*item)->type = HERE_DOC;
                (*i)++;
            }
            else
            {
                (*item)->value = ft_strdup("<");
                (*item)->type = IN_REDIR;
            }
            break;
        case '>':
            if (input[*i + 1] == '>')
            {
                (*item)->value = ft_strdup(">>");
                (*item)->type = APPEND_REDIR;
                (*i)++;
            }
            else
            {
                (*item)->value = ft_strdup(">");
                (*item)->type = OUT_REDIR;
            }
            break;
        default:
            free((*item)->next);
            (*item)->next = NULL;
            return;
    }
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
	item->type = WORD;
}
t_split	*remove_empty_nodes(t_split *item)
{
	t_split	*tmp;
	t_split	*start;

	if (!item)
        return NULL;
	tmp = item;
	start = item;
	if (tmp->value == '\0' && tmp->next == NULL)
	{
		free(start->value);
		free(start);
		return (NULL);
	}
	else if (item->value == '\0')
	{
		start = item->next;
		item = start;
		free(tmp->value);
		free(tmp);
	}
	while (item)
	{
		if (item->next && item->next->value == '\0')
		{
			tmp = item->next;
			item = item->next->next;
			free(tmp->value);
			free(tmp);
		}
		item = item->next;
	}
	return (start);
}

int check_operation(t_split *item)
{
    t_split *tmp;
    t_split *prev;

    prev = NULL;
    tmp = item;
    remove_empty_nodes(item);
    item = tmp;

    if (item && item->type == S_PIPE)
        return (1); // Error handling
    while (item && item->value)
    {
        if (item->next)
        {
            if (item->next->type == S_PIPE &&
                (item->type != WORD || (item->next->next && item->next->next->type != WORD)))
            {
                return (1); // Error handling
            }
            else if ((item->type == HERE_DOC || item->type == IN_REDIR || item->type == OUT_REDIR ||
                      item->type == APPEND_REDIR) &&
                     (item->next == NULL || item->next->type != WORD))
            {
                return (1); // Error handling
            }
        }
        item = item->next;
    }

    return (0); // No errors
}


void	tokenization(char *input, t_env	*env)
{
	int		i;
	int		start;
	int		end;
	int		quote_count;
	char	current_quote;
	t_split	*item;
	t_split	*tmp;
	t_minishell *minishell;

	item = ft_lstnew(NULL);
	if (!item)
		return ;
	tmp = item;
	i = 0;
	end = 0;
	start = 0;
	quote_count = 0;
	while (input[i])
	{
		if (input[i] == '"' || input[i] == 39)
		{
			quote_count++;
			if (i == 0 || (i > 0 && input[i - 1] == ' '))
				start = i;
			current_quote = input[i];
			i++;
			while (input[i] && input[i] != current_quote)
				i++;
			if (input[i] != current_quote)
			{
				exit(1 && write(2, "Error4\n", 7));
			}
			quote_count++;
			if (input[i] == current_quote && (input[i + 1] == '\0' || input[i + 1] == ' ' || input[i + 1] == '|' || input[i + 1] == '<' || input[i + 1] == '>'))
			{
				end = i;
				item->next = malloc(sizeof(t_split));
				if (!item->next)
					return ;
				ft_strcut(item->next, input, start, end);
				start = ++end;
				current_quote = 0;
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
			start = i;
			continue ;
		}
		if (i == 0 || (i > 0 && (input[i - 1] == ' ' || input[i - 1] == '|' || input[i - 1] == '>' || input[i - 1] == '<')))
			start = i;
		while (input[i] && !(input[i] == '"' || input[i] == 39) && input[i] != '|' && input[i] != '<' 
			&& input[i] != '>' && ((input[i] != ' ' && input[i] != '\t' && input[i] != '\n')))
			i++;
		if (input[i] == '"' || input[i] == 39)
		{
			quote_count++;
			current_quote = input[i];
			continue ;
		}
		end = i - 1;
		i--;
		item->next = malloc(sizeof(t_split));
		if (!item->next)
			return ;
		//printf("endddd: %d\n", end);
		ft_strcut(item->next, input, start, end);
		item = item->next;
		item->next = NULL;
		i++;
	}
	item = tmp->next;
	current_quote = 0;
	free(tmp);
	tmp = item;
	//printf("hmmmmm: %s$\n ----- type: %s", tmp->value, tmp->type);
	dollar_sign(item, env);
	//remove_empty_nodes(item);
	check_operation(item);
	minishell = malloc(sizeof(t_minishell));
	if (!minishell)
		return ;
	minishell->tokens = tmp;
	minishell->env = env;
	built_in(minishell);
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
