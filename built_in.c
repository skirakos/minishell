#include "minishell.h"

t_split	*pre_execve(t_minishell *minishell)
{
	t_split	*tmp;
	int		i;
	int		len;
	char	**matrix;

	tmp = minishell->tokens;
	len = 0;
	while (tmp && tmp->value && (tmp->type == WORD || tmp->type != S_PIPE))
	{
		len++;
		tmp = tmp->next;
	}
	tmp = minishell->tokens;
	matrix = (char **)malloc(sizeof(char *) * (len + 1));
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
		printf("matrix[%d] %s\n", i, matrix[i]);
		i++;
	}
	minishell->cmd = matrix;
	// printf("SSSSSSSSSSSSSSSSSSS------>%s\n", minishell->cmd[0]);
	printf("help-------->%p\n" , minishell->tokens);
	if (tmp->type == S_PIPE)
		minishell->tokens = minishell->tokens->next;
	return (minishell->tokens);
}
int	search_slash(char *str)
{
	int	i;

	i = 0;
	printf("slashhhhh ?? \n");
	printf("str slashghhh: %s\n", str);
	while (str[i])
	{
		printf("str[%d] == %c\n", i, str[i]);
		if (str[i] == '/')
			return (0);
		i++;
	}
	return (-1);
}
char	**env_to_matrix(t_minishell *minishell)
{
	int		len;
	t_env	*env;
	char	**envp;
	char	*join_tmp;
	char	*zibil;
	int		i;

	env = minishell->env;
	len = 0;
	while (minishell && minishell->env)
	{
		len++;
		minishell->env = minishell->env->next;
	}
	minishell->env = env;
	envp = (char **)malloc(sizeof(char *) * (len + 1));
	i = 0;
	while (minishell && minishell->env)
	{
		join_tmp = ft_strjoin(minishell->env->var, "=");
		zibil = join_tmp;
		join_tmp = ft_strjoin(join_tmp, minishell->env->value);
		free(zibil);
		envp[i]= malloc(ft_strlen(join_tmp) + 1);
		envp[i] = ft_strdup(join_tmp);
		free(join_tmp);
		join_tmp = NULL;
		i++;
		minishell->env = minishell->env->next;
	}
	envp[i] = NULL;
	minishell->env = env;
	return (envp);
}
void	check_cmd(t_minishell *minishell)
{
	t_env	*tmp;
	char	**path;
	char	*joined_path;
	char	*tmp_join;
	int		i;

	if (!minishell->cmd[0]) {
		printf("fbafhaifha\n");
		return ;
	}
	tmp = minishell->env;
	if ((minishell->cmd[0][0] == '.' && minishell->cmd[0][1] == '/') || search_slash(minishell->cmd[0]) == 0)
	{
		printf("slash gtav\n");
		if (access(minishell->cmd[0], X_OK) == 0)
			return ;
		else if (minishell->cmd[0][0] == '.' && minishell->cmd[0][1] == '/')
			return ;
	}
	if (minishell->cmd[0][0] == '.' && minishell->cmd[0][1] == '/')
	{
		//printf("slash gtav\n");
		if (access(minishell->cmd[0], X_OK) != 0)
			return ;
	}
	while (minishell->env)
	{
		//printf("env->value: %p\n", minishell->env->value);
		printf("esteghhhhhhhh??????\n");
		if (ft_strcmp(minishell->env->var, "PATH") == 0)
			path = ft_split(minishell->env->value, ':');
		minishell->env = minishell->env->next;
	}
	i = 0;
	while (path[i])
	{
		printf("path[%d] = %s\n", i, path[i]);
		i++;
	}
	
	i = 0;
	while (path[i])
	{
		if (ft_strcmp(minishell->cmd[0], "echo") != 0 && ft_strcmp(minishell->cmd[0], "cd") != 0 && ft_strcmp(minishell->cmd[0], "pwd") != 0 && ft_strcmp(minishell->cmd[0], "export") != 0 && ft_strcmp(minishell->cmd[0], "unset") != 0 && ft_strcmp(minishell->cmd[0], "env") != 0 && ft_strcmp(minishell->cmd[0], "exit") != 0)
		{
			joined_path = ft_strjoin(path[i], "/");
			tmp_join = joined_path;
			joined_path = ft_strjoin(joined_path, minishell->cmd[0]);
			printf("joined_path: %s\n", joined_path);
			free(tmp_join);
			if (access(joined_path, X_OK) == 0)
			{
				free(minishell->cmd[0]);
				minishell->cmd[0] = ft_strdup(joined_path);
				free(joined_path);
				free_matrix(path, matrix_len(path));
				minishell->env = tmp;
				return ;
			}
			free(joined_path);
		}
		i++;
	}
	minishell->env = tmp;
	free_matrix(path, matrix_len(path));
}
int	pipe_count(t_minishell *minishell)
{
	int		pipe_count;
	t_split *tmp;
	tmp = minishell->tokens;
	pipe_count = 0;
	while (minishell && minishell->tokens)
	{
		if (minishell->tokens->type == S_PIPE)
			pipe_count++;
		minishell->tokens = minishell->tokens->next;
	}
	minishell->tokens = tmp;
	return (pipe_count);
}
int handle_redirection(t_split *tokens) {
    int fd;
    t_split *current;

	current = tokens;
	fd = -1;
    while (current) {
        if (current->type == IN_REDIR)
		{
            if (ft_strcmp(current->value, ">") == 0)
                fd = open(current->next->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			else if (ft_strcmp(current->value, ">>") == 0)
                fd = open(current->next->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd == -1) {
                perror("open");
                return -1;
            }
            dup2(fd, STDOUT_FILENO);
            break;
        }
        current = current->next;
    }
	tokens = current;
    return fd;
}


void	built_in(t_minishell *minishell)
{
	t_split	*tmp;
	char	**envp;
	int		pipes;
	//int		pid;
	int		curr;

	//fd = open()
	curr = 0;
	tmp = minishell->tokens;
	pipes = pipe_count(minishell);
	printf("helloo builtin\n");
	printf("pipe: %d\n", pipes);
	while (minishell->tokens && minishell->tokens->value)
	{
		minishell->tokens = pre_execve(minishell);
		check_cmd(minishell);
		envp = env_to_matrix(minishell);
		printf("aa\n");
		printf("cmd[%d] = %s\n", 0, minishell->cmd[0]);
		
		if (curr < pipes + 1)
		{
			if (ft_strcmp(minishell->cmd[0], "cd") == 0)
			{
				printf("cd is found\n");
				cd(minishell);
				printf("b\n\n\n\n");
			}
			else if (ft_strcmp(minishell->cmd[0], "env") == 0)
			{
				printf("env is found\n");
				env(minishell);
			}
			else if (ft_strcmp(minishell->cmd[0], "unset") == 0)
			{
				printf("unset is found\n");
				unset(minishell);
				printf("after unset %s\n\n", minishell->env->var);
			}
			// pid = fork();
			// if (pid == 0)
			// {
			// 	printf("pid is zero\n");
			// 	handle_redirection(minishell->tokens);
			// 	printf("after redir\n");
			// 	if (ft_strcmp(minishell->cmd[0], "echo") == 0)
			// 	{
			// 		printf("echo is found\n");
			// 		echo(minishell->cmd);
			// 	}
			// 	else if (ft_strcmp(minishell->cmd[0], "pwd") == 0)
			// 	{
			// 		printf("pwd is found\n");
			// 		pwd();
			// 	}
			// 	else if (execve(minishell->cmd[0], minishell->cmd, envp) == -1)
			// 		perror("execve failed");
			//}
			curr++;
		}
		//printf("here ?? \n");
	//		waitpid(pid, NULL, 0);
	
		//continue;
		//minishell->tokens = minishell->tokens->next;
	}
	minishell->tokens = tmp;
}
