#include "minishell.h"

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
	while (input[i] && start <= end)
	{
		item->value[i] = input[start];
		i++;
		start++;
	}
	item->value[i] = '\0';
	//printf("%s\n", item->value);
	item->type = "word";
}

void	tokenization(char *input)
{
	int		i;
	int		start;
	int		end;
	int		quotes;
	t_split	*item;
	t_split	*tmp;

	item = ft_lstnew(NULL, NULL);
	if (!item)
		return ;
	tmp = item;
	i = 0;
	quotes = 0;
	while (input[i])
	{
		if (input[i] == '"')
		{
			quotes++;
			start = i;
			i++;
			while (input[i] && input[i] != '"')
				i++;
			if (input[i] == '"' && (quotes % 2) == 1 && input[i + 1] && (input[i + 1] == ' ' || input[i + 1] == '\0'))
			{
				printf("bebebebe\n");
				end = i;
				item->next = malloc(sizeof(t_split));
				if (!item->next)
					return ;
				printf("start - %d, end - %d \n", start, end);
				ft_strcut(item->next, input, start, end);
				item = item->next;
				item->next = NULL;
				i++;
				quotes++;
				continue ;
			}
			else if (input[i] == '"' && (quotes % 2) == 1)
			{
				quotes++;
				while (input[i])
				{
					printf("%d\n", quotes);
					while (input[i] && input[i] != ' ' && (quotes % 2) == 0)
					{
						i++;
						printf("%d ----> %c\n", i,input[i]);
						if (input[i] == '"')
						{
							i++;
							quotes++;
						}
					}
					printf("%d\n", quotes);
					while (input[i] && input[i] != '"' && (quotes % 2) == 1)
					{
						printf("%d ----> %c\n", i,input[i]);
						i++;
						if (input[i] == '"')
						{
							i++;
							quotes++;
						}
					}
						printf("%d ----> %c\n", i,input[i]);
					printf("%d\n", quotes);
					end = i;
					while (input[i] && input[i] != ' ' && (quotes % 2) != 0)
					{
						i++;
						printf("%d ----> %c\n", i,input[i]);
						if (input[i] == '"')
						{
							i++;
							quotes++;
						}
					}
					end = i;
					printf("%d\n", quotes);
					if ((quotes % 2) == 0 && (input[i] == '\0' || input[i] == ' '))
					{
						item->next = malloc(sizeof(t_split));
						if (!item->next)
						return ;
						printf("start - %d, end - %d \n", start, end);
						ft_strcut(item->next, input, start, end);
						item = item->next;
						item->next = NULL;
						i++;
						break ;
					}
					else if (input[i] == '"')
					{
						i++;
						quotes++;
					}
				}
				continue;
			}
		}
		while (input[i] && (input[i] == ' ' || input[i] == '\t' || input[i] == '\n') && input[i] != '"')
			i++;
		if ((input[i] == '|' || input[i] == '<' || input[i] == '>') && (input[i] != '"'))
		{
			type_operator(&item, input, &i);
			i++;
			continue ;
		}
		start = i;
		printf("stopy: %c\n", input[i]);
		while (input[i] && input[i] != '"' && input[i] != '|' && input[i] != '<' 
			&& input[i] != '>' && ((input[i] != ' ' && input[i] != '\t' && input[i] != '\n')))
			i++;
		if (input[i] == '"' && start == i)
			continue ;
		end = i - 1;
		i--;
		item->next = malloc(sizeof(t_split));
		if (!item->next)
			return ;
		printf("start - %d, end - %d \n", start, end);
		ft_strcut(item->next, input, start, end);
		item = item->next;
		item->next = NULL;
		i++;
	}
	printf("quote:%d\n", quotes);
	if (quotes % 2 != 0)
		exit(1 && write(2, "Error\n", 6));
	item = tmp->next;
	free(tmp);
	tmp = item;
	while (tmp && tmp->value)
	{
		printf("%s\n", tmp->value);
		tmp = tmp->next;
	}
}
