#include "minishell.h"

void	unset(t_minishell *minishell)
{
	t_env	*tmp;
	t_env	*prev;
	t_env	*node_to_delete;

	if (!minishell->env || !minishell->cmd[1])
        return;
	if (ft_strcmp(minishell->cmd[1], minishell->env->var) == 0)
	{
		tmp = minishell->env->next;
		free(minishell->env->value);
		free(minishell->env->var);
		free(minishell->env);
		minishell->env = tmp;
		return ;
	}
	tmp = minishell->env;
	prev = minishell->env;
	minishell->env = minishell->env->next;
	while (minishell->env)
	{
		if (ft_strcmp(minishell->cmd[1], minishell->env->var) == 0)
		{
			node_to_delete = minishell->env;
			prev->next = minishell->env->next;
			free(node_to_delete->var);
			free(node_to_delete->value);
			free(node_to_delete);
			printf("bn\n");
			break ;
		}
		prev = minishell->env;
		minishell->env = minishell->env->next;
	}
	minishell->env = tmp;
	//last node case
}

void	env(t_minishell *minishell)
{
	t_env	*tmp;

	tmp = minishell->env;
	while (minishell->env)
	{
		ft_putstr_fd(minishell->env->var, 1);
		ft_putstr_fd("=", 1);
		ft_putstr_fd(minishell->env->value, 1);
		ft_putstr_fd("\n", 1);
		minishell->env = minishell->env->next;
	}
	minishell->env = tmp;
}

void	pwd()
{
	char	pwd[PATH_MAX];

	if (!getcwd(pwd, PATH_MAX))
		return ; //Error handling
	ft_putstr_fd(pwd, 1);
	ft_putstr_fd("\n", 1);
}