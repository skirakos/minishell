#include "minishell.h"

void	type_operator(t_split **item, char *input, int i)
{
	(*item)->next = malloc(sizeof(t_split));
	if (!(*item)->next)
		return ;
	(*item) = (*item)->next;
	if (input[i] == '|')
	{
		(*item)->value = "|";
		(*item)->type = "pipe";
	}
	else if (input[i] == '<')
	{
		if (input[i] && input[i + 1] == '<')
		{
			(*item)->value = "<<";
			(*item)->type = "heredoc";
		}
		(*item)->value = "<";
		(*item)->type = "outredir";
	}
	else if (input[i] == '>')
	{
		if (input[i] && input[i + 1] == '>')
		{
			(*item)->value = ">>";
			(*item)->type = "append";
		}
		(*item)->value = ">";
		(*item)->type = "inredir";
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
	while (input[i] && start <= end)
	{
		item->value[i] = input[start];
		i++;
		start++;
	}
	item->value[i] = '\0';
	item->type = "word";
}

void	tokenization(char *input)
{
	int		i;
	int		start;
	int		end;
	t_split	*item;
	t_split	*tmp;

	item = malloc(sizeof(t_split));
	if (!item)
		return ;
	tmp = item;
	i = 0;
	while (input[i])
	{
		while (input[i] && (input[i] == ' ' || input[i] == '\t' || input[i] == '\n'))
			i++;
		if (input[i] == '|' || input[i] == '<' || input[i] == '>')
		{
			type_operator(&item, input, i);
			i++;
			continue ;
		}
		start = i;
		while (input[i] && input[i] != '|' && input[i] != '<' 
			&& input[i] != '>' && ((input[i] != ' ' && input[i] != '\t' && input[i] != '\n')))
			i++;
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
	while (tmp)
	{
		printf("%s\n", tmp->value);
		tmp = tmp->next;
	}
}
