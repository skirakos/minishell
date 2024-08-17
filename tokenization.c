#include "minishell.h"

void	type_operator(t_split **item, char *input, int *i)
{
	(*item)->next = malloc(sizeof(t_split));
	if (!(*item)->next)
		return ;
	(*item) = (*item)->next;
	if (input[*i] == '|')
	{
		//printf("b\n");
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
	//printf("index after operation: %d\n", *i);
	// if (input[*i + 1] && input[*i + 1] == ' ')
	//(*i)++;
	(*item)->next = NULL;
}

void	ft_strcut(t_split *item, char *input, int start, int end)
{
	int	i;

	i = 0;
	printf("start: %d, end: %d\n", start, end);
	printf("allocation size: %d\n", end - start + 2);
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
			//printf("hello\n");
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
			if (input[i] == current_quote && (input[i + 1]== '\0' || input[i + 1] == ' ' || input[i + 1] == '|' || input[i + 1] == '<' || input[i + 1] == '>'))
			{
				end = i;
				item->next = malloc(sizeof(t_split));
				if (!item->next)
					return ;
				ft_strcut(item->next, input, start, end);
				start = end+2;
				item = item->next;
				item->next = NULL;
				i++;
				//printf("%c\n", input[i]);
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
		if ((input[i] == '|' || input[i] == '<' || input[i] == '>') && (input[i] != '"'))
		{
			//printf("index: %d\n", i);
			type_operator(&item, input, &i);
			i++;
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
	while (tmp && tmp->value)
	{
		printf("%s\n", tmp->value);
		tmp = tmp->next;
	}
	dollar(item, env);
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
				if (check_key_in_env(env, key) == 1)
				{
					printf("gtela\n");
				}
			}
			else
				i++;
		}
		item = item->next;
	}
}

int	check_key_in_env(t_env *env, char *key)
{
	while (env)
	{
		if (!ft_strncmp(env->var, key, ft_strlen(env->var)))
		{
			printf("%s\n", env->value);
			return (1);
		}
		env = env->next;
	}
	return (-1);
}
