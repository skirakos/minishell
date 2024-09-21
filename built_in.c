#include "minishell.h"

t_split	*pre_execve(t_minishell *minishell)
{
	t_split	*tmp;
	int		i;
	int		len;
	char	**matrix;

	tmp = minishell->tokens;
	i = 0;
	while (tmp && tmp->value && tmp->type == WORD && tmp->type != S_PIPE)
	{
		i++;
		tmp = tmp->next;
	}
	tmp = minishell->tokens;
	len = i;
	matrix = (char **)malloc(sizeof(char *) * (i + 1));
	if (!matrix)
		return (NULL);
	i = 0;
	while (minishell->tokens && minishell->tokens->value && i < len)
	{
		matrix[i] = ft_strdup(minishell->tokens->value);
		i++;
		minishell->tokens = minishell->tokens->next;
	}
	matrix[len] = NULL;
	i = 0;
	while (matrix[i] != NULL)
	{
		printf("hello\n");
		printf("matrix[%d] %s\n", i, matrix[i]);
		i++;
	}
	minishell->cmd = matrix;
	return (minishell->tokens);
}

void	built_in(t_minishell *minishell)
{
	t_split	*tmp;

	tmp = minishell->tokens;
	//printf("helloo builtin\n");

	while (minishell->tokens && minishell->tokens->value)
	{
		// if (ft_strcmp(tokens->value, "echo") == 0 || ft_strcmp(tokens->value, "/bin/echo") == 0 || ft_strcmp(tokens->value, "ECHO") == 0)
		// {
			
		// }
		// else if (ft_strcmp(tokens->value, "cd") == 0 || ft_strcmp(tokens->value, "/bin/cd") == 0 || ft_strcmp(tokens->value, "CD") == 0)
		// {

		// }
		// else if (ft_strcmp(tokens->value, "unset") == 0 || ft_strcmp(tokens->value, "/bin/unset") == 0 || ft_strcmp(tokens->value, "UNSET") == 0)
		// {

		// }
		// else if (ft_strcmp(tokens->value, "pwd") == 0 || ft_strcmp(tokens->value, "/bin/pwd") == 0 || ft_strcmp(tokens->value, "PWD") == 0)
		// {

		// }
		// else if (ft_strcmp(tokens->value, "export") == 0 || ft_strcmp(tokens->value, "/bin/export") == 0 || ft_strcmp(tokens->value, "EXPORT") == 0)
		// {

		// }
		// else if (ft_strcmp(tokens->value, "env") == 0 || ft_strcmp(tokens->value, "/bin/env") == 0 || ft_strcmp(tokens->value, "ENV") == 0)
		// {

		// }
		// else if (ft_strcmp(tokens->value, "exit") == 0 || ft_strcmp(tokens->value, "/bin/exit") == 0 || ft_strcmp(tokens->value, "EXIT") == 0)
		// {

		// }
		// else
		// {
		// 	//printf("hello pre_excve\n");
		// 	//continue;
		// }
		minishell->tokens = pre_execve(minishell);
		//continue;
		//tokens = tokens->next;
	}
	minishell->tokens = tmp;
}
