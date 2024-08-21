#include "minishell.h"

void	get_value(t_env **copy, char **env, int i, int *j)
{
	int	k;

	while (env[i][*j])
	{
		(*copy)->value = (char *)malloc(sizeof(char) * (ft_strlen(env[i]) - ft_strlen((*copy)->var) + 2));
		if (!(*copy)->value)
			return ;
		k = 0;
		while (env[i][*j])
		{
			(*copy)->value[k] = env[i][*j];
			k++;
			(*j)++;
		}
		(*copy)->value[k] = '\0';
	}
}
t_env	*ft_lstnew_env(char *content, char *type)
{
	t_env	*n;

	n = malloc(sizeof(t_env));
	if (!n)
		return (NULL);
	n->var = content;
	n->value = type;
	n->next = NULL;
	return (n);
}

t_env	*env_copy(char **env)
{
	t_env	*copy;
	t_env	*tmp;
	int		i;
	int		j;

	i = 0;
	copy = ft_lstnew_env(NULL, NULL);
	if (!copy)
		return (NULL);
	tmp = copy;
	while (env[i])
	{
		j = 0;
		copy->next = ft_lstnew_env(NULL, NULL);
		copy = copy->next;
		while (env[i][j] != '=')
			j++;
		if (env[i][j] == '=')
		{
			//...printf("%d\n", j);
			copy->var = (char *)malloc(sizeof(char) * (j + 1));
			if (!copy->var)
				return (NULL);
			int	k = 0;
			while (k < j)
			{
				copy->var[k] = env[i][k];
				k++;
			}
			copy->var[k] = '\0';
			//printf("aaaa\n");
		}
		j++;
		get_value(&copy, env, i, &j);
		//printf("skjhfjks\n");
		i++;
	}
	copy = tmp->next;
	free(tmp);
	return (copy);
}

int main(int argc, char **argv, char **env)
{
	char	*input = NULL;
	t_env	*fake_env;

	(void)argv;

	if (argc == 1)
	{
		fake_env = env_copy(env);
		while (1)
		{
			input = readline("MINISHELL GJUK: ");
			if (input == NULL)
			{
				free(input);
				break;
			}
			else
			{
				add_history(input);
				tokenization(input, fake_env);
				free(input);
			}
		}
	}
	if (input)
		free(input);
	return (0);
}