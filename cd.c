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
		path = getenv("HOME");
	else if (minishell->cmd[1][0] == '~')
		path = tilda(minishell);
	else
		path = minishell->cmd[1];
	if (access(path, F_OK) == -1)
		print_err(1, "minishell: cd: ", path, ": No such file or directory\n");
	else if (!is_directory(path))
		print_err(1, "minishell: cd: ", path, ": Not a directory\n");
	else if (access_directory(path) == 0)
		print_err(1, "minishell: cd: ", path, ": Permission denied\n");
	else if (access(path, F_OK) == 0)
	{
		chdir(path);
		if (getcwd(new_pwd, PATH_MAX) != NULL)
			update_env(minishell->env, new_pwd, 1);
		g_exit_status = 0;
	}
}

int	access_directory(const char	*path)
{
	if (access(path, R_OK | X_OK) != 0)
		return (0);
	else
		return (1);
}

bool	is_directory(const char	*path)
{
	struct stat	path_stat;

	if (stat(path, &path_stat) != 0)
		return (false);
	return (S_ISDIR(path_stat.st_mode));
}
