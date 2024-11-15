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
int	contains_char(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == c)
			return (1);
		i++;
	}
	return (0);
}

int	is_valid_var(char *var)
{
	int	i;

	i = 0;
	while (var[i] != '\0')
	{
		if (!((var[i] >= 'a' && var[i] <= 'z')
			 || (var[i] >= 'A' && var[i] <= 'Z')
			 || (var[i] >= '0' && var[i] <= '9')
			 || (var[i] == '+' && var[i + 1] == '\0')
			 || var[i] == '_'))
			 return (1);
		i++;
	}
	return (0);
}

char **str_divide_by_equal_sign(char *str)
{
	int		j;
	int		i;
	char	**res;

	i = 0;
	res = (char **)malloc(3 * sizeof(char *));
	if (!res)
		return (NULL);
	while (str[i] && str[i] != '=')
		i++;
	res[0] = malloc(i + 1);
	res[1] = malloc(ft_strlen(str) - i);
	if (!res[0] || !res[1])
	{
		free(res[0]);
		free(res[1]);
		free(res);
		return (NULL);
	}
	i = 0;
	while (str[i] && str[i] != '=')
	{
		res[0][i] = str[i];
		i++;
	}
	res[0][i] = '\0';
	j = 0;
	if (str[i] && str[i] == '=')
	{
		i++;
		while (str[i])
		{
			res[1][j] = str[i++];
			j++;
		}
		res[1][j] = '\0';
	}
	if (j == 0)
	{
		free(res[1]);
		res[1] = NULL;
	}
	res[2] = NULL;
	return (res);
}

void	exec_export(t_minishell *minishell)
{
	t_env	*tmp;
	char	**split;
	char	*new_value;
	int		i;

	i = 1;
	while (minishell->cmd[i])
	{
		tmp = minishell->env;
		if(minishell->cmd[i][0] == '=' || is_digit(minishell->cmd[i][0]))
		{
			print_err(1, "minishell: export: `", minishell->cmd[i], "': not a valid identifier\n");
			i++;
			continue;
		}
		split = str_divide_by_equal_sign(minishell->cmd[i]);
		if (is_valid_var(split[0]))
		{
			print_err(1, "minishell: export: `", minishell->cmd[i], "': not a valid identifier\n");
			i++;
			continue;
		}
		if (contains_char(split[0], '+'))
		{
			split[0][ft_strlen(split[0]) - 1] = '\0';
			if (!check_in_env(minishell->env, split[0]))
			{
				while (ft_strcmp(tmp->next->var, split[0]) != 0)
				{
					printf("%s\n",tmp->next->var);
					tmp = tmp->next;
				}
				if (!ft_strcmp(tmp->next->value, "\"\"") && split[1])
				{
					free(tmp->next->value);
					tmp->next->value = NULL;
				}
				new_value = ft_strjoin(tmp->next->value, split[1]);
				free(tmp->next->value);
				tmp->next->value = new_value;
				tmp->next->next = NULL;
				tmp = tmp->next;
			}
			else
			{
				while (tmp->next)
				tmp = tmp->next;
				tmp->next = malloc(sizeof(t_env));
				tmp->next->var = ft_strdup(split[0]);
				tmp->next->value = ft_strdup(split[1]);
				tmp->next->next = NULL;
				tmp = tmp->next;
			}
		}
		else if (matrix_len(split) == 1 && check_in_env(minishell->env, split[0]))
		{
			while (tmp->next)
				tmp = tmp->next;
			tmp->next = malloc(sizeof(t_env));
			tmp->next->var = ft_strdup(split[0]);
			if (contains_char(minishell->cmd[i], '=') == 1)
				tmp->next->value = ft_strdup("\"\"");
			else
				tmp->next->value = NULL;
			tmp->next->next = NULL;
			tmp = tmp->next;
		}
		else if (matrix_len(split) == 1 && !check_in_env(minishell->env, split[0]))
		{
			while (ft_strcmp(tmp->next->var, split[0]) != 0)
				tmp = tmp->next;
			if (contains_char(minishell->cmd[i], '=') == 1)
			{
				free(tmp->next->value);
				tmp->next->value = ft_strdup("\"\"");
			}
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
				if (!envp[i][j + 1] && flag == 0)
					break;
				else if (envp[i][j + 1] != '"' && flag == 0)
				{
					printf("=\"");
					flag = 1;
					j++;
				}
				else if (envp[i][j + 1] == '"')
				{
					printf("=");
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
