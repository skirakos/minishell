#include "minishell.h"

void	update_env(t_env *env, char *pwd_to_set, int flag)
{
	t_env	*tmp;
	char	*search;

	tmp = env;
	if (flag == 0)
		search = "OLDPWD";
	else if (flag == 1)
		search = "PWD";
	while (env)
	{
		if (ft_strcmp(env->var, search) == 0)
		{
			free(env->value);
			env->value = NULL;
			env->value = ft_strdup(pwd_to_set);
			break;
		}
		env = env->next;
	}
	env = tmp;
	// while (env)
	// {
	// 	printf("%s=%s\n", env->var, env->value);
	// 	env = env->next;
	// }
	// env = tmp;
}

char	*tilda(t_minishell *minishell)
{
	char	*path;
	char	*home_path;

	path = minishell->cmd[1] + 1;
	home_path = ft_strdup(getenv("HOME"));
	path = ft_strjoin(home_path, path);
	free(home_path);
	//printf("path: %s\n", path);
	return (path);
}

void	cd(t_minishell *minishell)
{
	char	*path;
	char	old_pwd[PATH_MAX];
	char	new_pwd[PATH_MAX];

	if (getcwd(old_pwd, PATH_MAX) != NULL)
		update_env(minishell->env, old_pwd, 0);
	if (!minishell->cmd[1] || ft_strcmp(minishell->cmd[1], "~") == 0 || ft_strcmp(minishell->cmd[1], "~/") == 0)
	{	path = getenv("HOME");
		perror_exit(g_exit_status, "cd: ");
	}
	else if (minishell->cmd[1][0] == '~')
		path = tilda(minishell);
	else
		path = minishell->cmd[1];
	if (access(path, F_OK) == 0)
	{
		chdir(path);
		if (getcwd(new_pwd, PATH_MAX) != NULL)
			update_env(minishell->env, new_pwd, 1);
	}
	else
		perror_exit(g_exit_status, "cd: ");
}
