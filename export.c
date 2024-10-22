#include "minishell.h"

int	check_in_env(t_env *env, char *str)
{
	t_env	*tmp;

	tmp = env;
	while(env && env->var)
	{
		if (ft_strcmp(env->var, str) == 0)
			return 0;
		env = env->next;
	}
	env = tmp;
	return (1);
}

void	exec_export(t_minishell *minishell)
{
	t_env	*tmp;
	char	**split;
	int		i;

	i = 1;
	tmp = minishell->env;
	while (minishell->cmd[i])
	{
		split = ft_split(minishell->cmd[i], '=');
		printf("split[0]=%s --- split[1]=%s\n", split[0], split[1]);
		printf("matrix_len(split)=%d\n", matrix_len(split));
		if (matrix_len(split) == 1 && !check_in_env(minishell->env, split[0]))
		{
			printf("i am here1\n");
			while (tmp->next)
				tmp = tmp->next;
			tmp->next = malloc(sizeof(t_env)); 
			tmp->next->var = ft_strdup(split[0]);
			tmp->next->value = NULL;
			tmp->next->next = NULL;
			tmp = tmp->next;
		}
		else if (matrix_len(split) == 2 && check_in_env(minishell->env, split[0]) == 1)
		{
			printf("i am here2\n");
			while (tmp->next)
				tmp = tmp->next;
			tmp->next = malloc(sizeof(t_env));
			tmp->next->var = ft_strdup(split[0]);
			tmp->next->value = ft_strdup(split[1]);
			tmp->next->next = NULL;
			tmp = tmp->next;
		}
		else if (matrix_len(split) == 2 && check_in_env(minishell->env, split[0]) == 0)
		{
			while (tmp->next && ft_strcmp(split[0], tmp->var) != 0)
				tmp = tmp->next;
			//printf("value=%s\n", tmp->next->value);
			if (tmp->value)
				free(tmp->value);
			tmp->value = ft_strdup(split[1]);
			tmp = tmp->next;
		}
		i++;
	}
	
}

void	export_bulki(t_minishell *minishell, char **envp)
{
	int	i;

	if (matrix_len(minishell->cmd) == 1)
	{
		merge_sort(envp, 0, matrix_len(envp) - 1);
		i = 0;
		while (envp[i])
		{
			printf("declare -x %s\n", envp[i]);
			i++;
		}
	}
	else if (matrix_len(minishell->cmd) > 1)
	{
		exec_export(minishell);
	}
}