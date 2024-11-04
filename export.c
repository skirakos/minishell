#include "minishell.h"

int	check_in_env(t_env *env, char *str)
{
	t_env	*tmp;

	tmp = env;
	while(env && env->var)
	{
		if (ft_strcmp(env->var, str) == 0)
			return (0);
		env = env->next;
	}
	env = tmp;
	return (1);
}
int	is_digit(char c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
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
		if(minishell->cmd[i][0] == '=' || is_digit(minishell->cmd[i][0]))
		{
			print_err(1, "minishell: export: `", minishell->cmd[i], "': not a valid identifier");
			i++;
			continue;
		}
		split = ft_split(minishell->cmd[i], '=');
		if (matrix_len(split) == 1 && check_in_env(minishell->env, split[0]))
		{
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
		free_matrix(split, matrix_len(split));
	}
}
void	print_env(char **envp)
{
	int	i;
	int	j;
	int	flag;

	i = 0;
	while (envp[i])
	{
		j = 0;
		flag = 0;
		printf("declare -x ");
		while (envp[i][j])
		{
			if (envp[i][j] == '=')
			{
				if (!envp[i][j + 1])
					break;
				else
				{
					printf("=\"");
					flag = 1;
					j++;
				}
			}
			printf("%c", envp[i][j]);
			j++;
		}
		if (flag == 1)
			printf("\"");
		printf("\n");
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
		print_env(envp);
		// while (envp[i])
		// {
		// 	printf("declare -x %s\n", envp[i]);
		// 	i++;
		// }
	}
	else if (matrix_len(minishell->cmd) > 1)
	{
		exec_export(minishell);
	}
}
