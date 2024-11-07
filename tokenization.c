#include "minishell.h"

char	*get_from_env(t_env *env, char *key)
{
	char	*res;

	while (env)
	{
		// if (!ft_strncmp(env->var, key, ft_strlen(env->var)))
		if (!ft_strcmp(env->var, key))
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
			// printf("res: %s\n", res);
			return (res);
		}
		env = env->next;
	}
	return (NULL);
}

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
	if (str[i] && str[i] == '?')
		return (merge(before_key, ft_itoa(g_exit_status), ft_substr(str, i + 1, ft_strlen(str))));
	//printf("hres->%s\n", str);
	while (str[i] && (str[i] != ' ' && str[i] != '=' && str[i] != '/' && str[i] != '$' && str[i] != '"' && str[i] != 39 && str[i] != '>' && str[i] != '<' && str[i] != '>' && str[i] != '\0'))
		(i)++;
	end = (i) - 1;
	//printf("enddddd: %d\n", end);
	key = ft_substr(str, start, end + 1);
	//printf("i: %s\n", key);
	after_key = ft_substr(str, end + 1, ft_strlen(str));
	dollar_value = get_from_env(env, key);
	//printf("vata->%s\n", dollar_value);
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
				if (str[i + 1] == '\0' || (is_dquote && str[i + 1] == '"'))
				{
					printf("hsbahj\n");
					str = ft_strdup("$");
					break;
				}
				else if (!prev || (prev && prev->type != HERE_DOC))
				{
					printf("hyy? --- %c\n", str[i+1]);
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

    if (input[*i] == '|') 
	{
		(*item)->value = ft_strdup("|");
		(*item)->type = S_PIPE;
	} 
	else if (input[*i] == '<') 
	{
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
	} 
	else if (input[*i] == '>') 
	{
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
	} 
	else 
	{
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
	while (input[i] && start <= end)
	{
		item->value[i] = input[start];
		i++;
		start++;
	}
	item->value[i] = '\0';
	item->type = WORD;
}
// t_split	*remove_empty_nodes(t_split *item)
// {
// 	t_split	*tmp;
// 	t_split	*start;

// 	if (!item)
//         return NULL;
// 	tmp = item;
// 	start = item;
// 	if (tmp->value[0] == '\0' && tmp->next == NULL)
// 	{
// 		free(start->value);
// 		free(start);
// 		return (NULL);
// 	}
// 	else if (item->value[0] == '\0')
// 	{
// 		start = item->next;
// 		item = start;
// 		free(tmp->value);
// 		free(tmp);
// 	}
// 	while (item && item->next)
// 	{
// 		printf("seggggg??? \n");
// 		if (item->next && item->next->value[0] == '\0')
// 		{
// 			tmp = item->next;
// 			item = item->next->next;
// 			free(tmp->value);
// 			free(tmp);
// 		}
// 		else
// 			item = item->next;
// 	}
// 	//free(item->value);
// 	return (start);
// }



t_split	*remove_empty_nodes(t_split *item)
{
    t_split *current = item;
    t_split *prev = NULL;
    t_split *head = item;
    // Handle the case where the head (and consecutive nodes) are empty or NULL
    while (current != NULL && (current->value == NULL || current->value[0] == '\0')) {
        t_split *temp = current;
        current = current->next;
        free(temp->value);
        free(temp);
    }
    if (current == NULL) {
	    return NULL;
    }

    head = current;
    prev = current;
    current = current->next;
    while (current != NULL) {
        if (current->value == NULL || current->value[0] == '\0')
		{
            prev->next = current->next;
            free(current->value);
            free(current);
            current = prev->next;
        } else {
            prev = current;
            current = current->next;
        }
    }
    return head;
}

int check_operation(t_split **item)
{
	t_split	*temp;

	//printf("after remove empty nodes \n");
	// while (tmp && tmp->value)
	// {
	// 	printf("after remove value:%s$\n", tmp->value);
	// 	tmp = tmp->next;
	// }
    (*item) = remove_empty_nodes((*item));
    if ((*item) && (*item)->type == S_PIPE)
        return (1); // Error handling
	temp = *item;
    while (temp && temp->value)
    {
        if (temp->next)
        {
            if (temp->next->type == S_PIPE &&
                (temp->type != WORD || (temp->next->next && temp->next->next->type != WORD)))
            {
                return (1); // Error handling
            }
            else if ((temp->type == HERE_DOC || temp->type == IN_REDIR || temp->type == OUT_REDIR ||
                      temp->type == APPEND_REDIR) &&
                     (temp->next == NULL || temp->next->type != WORD))
            {
                return (1); // Error handling
            }
        }
        temp = temp->next;
    }
    return (0); // No errors
}


void	tokenization(char *input, t_minishell *minishell)
{
	int		i;
	int		start;
	int		end;
	//int		//quote_count;
	char	current_quote;
	t_split	*item;
	t_split	*tmp;

	item = ft_lstnew(NULL);
	if (!item)
		return ;
	tmp = item;
	i = 0;
	end = 0;
	start = 0;
	//quote_count = 0;
	while (input && input[i])
	{
		if (input[i] == '"' || input[i] == 39)
		{
			//quote_count++;
			if (i == 0 || (i > 0 && input[i - 1] == ' '))
				start = i;
			current_quote = input[i];
			i++;
			while (input[i] && input[i] != current_quote)
				i++;
			if (input[i] != current_quote)
			{
				//free(input);
				perror_exit(QUOTE_ERR, NULL);
				return ;
			}
			//quote_count++;
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
			//quote_count++;
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
	if (item)
	{
		dollar_sign(item, minishell->env);
		check_operation(&item);
		tmp = item;
		//printf("before syntax_check\n");
		if (syntax_check(item) == 1)
			return ;
		//printf("after syntax_check\n");
		item = tmp;
		// free(tmp);
		//tmp = item;
		minishell->tokens = item;
		if (minishell->tokens && minishell->tokens->value){
			//printf("here>>\n");
			built_in(minishell);
		}
		// i = 0;
		// while (tmp)
		// {
		// 	printf("%s$\n", tmp->value);
		// 	tmp = tmp->next;
		// }
	}
}

int	check_key_in_env(t_env *env, char *key, t_split *item)
{
	while (env)
	{
		if (!ft_strncmp(env->var, key, ft_strlen(env->var)))
		{
			int	i = 0;
			while (item->value[i] && env->value[i])
			{
				item->value[i] = env->value[i];
				i++;
			}
			return (1);
		}
		env = env->next;
	}
	free(item->value);
	item->value = NULL;
	return (-1);
}
