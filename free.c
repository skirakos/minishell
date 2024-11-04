#include "minishell.h"

void	free_t_split(t_split *tokens)
{
	t_split	*tmp;

	tmp = NULL;
	printf("%p\n",tokens);
	while (tokens)
	{
		tmp = tokens;
		// if (tokens && tokens->value)
		// 	free(tokens->value);
		tokens = tokens->next;
		tmp->next = NULL;
		free(tmp);
		tmp = NULL;
	}
	// free(tokens);
}

void	free_t_env(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env;
		free(env->value);
		env = env->next;
		free(tmp);
	}
	free(env);
}

int		matrix_len(char **matrix)
{
	int	i;

	i = 0;
	while (matrix[i])
	{
		i++;
	}
	return (i);
}

void	*free_matrix(char** matrix, int rows) {
    int 	i;

	i = 0;
    while (i < rows)
	{
        free(matrix[i]);
        i++;
    }
    free(matrix);
	return (NULL);
}

void	free_before_exit(t_minishell *minishell)
{
	if (minishell->tokens)
		free_t_split(minishell->tokens);
	if (minishell->env)
		free_t_env(minishell->env);
	if (minishell->cmd)
		free_matrix(minishell->cmd, matrix_len(minishell->cmd));
	if (minishell)
		free(minishell);
}