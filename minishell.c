#include "minishell.h"

int	g_exit_status = 0;

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
	char		*input;
	t_minishell	*minishell;
	
	(void)argv;
	input = ft_strdup("");
	if (argc == 1)
	{
		minishell = malloc(sizeof(t_minishell));
		if (!minishell)
			return (1);
		minishell->env = env_copy(env);
		minishell->fd_in = 0;
		minishell->fd_out = 1;
		minishell->fd_heredoc = 0;
		while (input)
		{
			//int g_exit_status = 0;
			signals();
			input = readline("\033[1;90m⚙️  MINISHELL GJUK\033[0m: ");
			if(input)
			{
				//printf("input: %s$\n", input);
				add_history(input);
				tokenization(input, minishell);
			}
			free(input);
		}
	}
	return (0);
}
