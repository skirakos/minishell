#include "minishell.h"

t_split	*pre_execve(t_minishell *minishell)
{
	t_split	*tmp;
	int		i;
	int		len;
	char	**matrix;

	tmp = minishell->tokens;
	len = 0;
	while (minishell->tokens && minishell->tokens->value && (minishell->tokens->type == WORD || minishell->tokens->type != S_PIPE))
	{
		if (check_redir(minishell->tokens->value))
			len -= 2;
		len++;
		minishell->tokens = minishell->tokens->next;
		printf("tokens->%p\n",minishell->tokens);
	}
	matrix = (char **)malloc(sizeof(char *) * (len + 1));
	if (!matrix)
		return (NULL);
	i = 0;

	printf("ste1\n");
	while (tmp && tmp->value && tmp->type != S_PIPE)
	{
		if (!check_redir(tmp->value))
		{
			matrix[i] = ft_strdup(tmp->value);
			i++;
		}
		else
		{
			printf("vabshe stexi durs galis es?\n");
			handle_redirection(minishell, tmp->type, tmp->next->value);
			printf("iharke voch\n");
			tmp = tmp->next;
		}
		tmp = tmp->next;
	}
	printf("ste2\n");
	matrix[len] = NULL;
	i = 0;
	while (matrix[i] != NULL)
	{
		printf("matrix[%d] %s\n", i, matrix[i]);
		i++;
	}
	minishell->cmd = matrix;
	// printf("SSSSSSSSSSSSSSSSSSS------>%s\n", minishell->cmd[0]);
	//printf("help-------->%p\n" , minishell->tokens);
	if (tmp && tmp->type == S_PIPE)
		minishell->tokens = minishell->tokens->next;
	printf("minishell->tokens: %p\n", minishell->tokens);
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
	printf("\n\n\n%s\n\n\n\n", minishell->env->var);
	return (envp);
}

int	is_builtin(char	*cmd)
{
	if (ft_strcmp(cmd, "echo") == 0
	|| ft_strcmp(cmd, "cd") == 0
	|| ft_strcmp(cmd, "pwd") == 0
	|| ft_strcmp(cmd, "export") == 0
	|| ft_strcmp(cmd, "unset") == 0
	|| ft_strcmp(cmd, "env") == 0
	|| ft_strcmp(cmd, "exit") == 0)
		return (0);
	return (1);
}
void	check_cmd(t_minishell *minishell)
{
	t_env	*tmp;
	char	**path;
	char	*joined_path;
	char	*tmp_join;
	int		i;

	if (!minishell->cmd[0]) {
		//printf("fbafhaifha\n");
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
		// printf("\nenv->afafafafe: %p\n\n", minishell->env);
		// printf("\nenv->fafafalue: %p\n\n", minishell->env->var);
		// printf("esteghhhhhhhh??????\n");
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
		if (is_builtin(minishell->cmd[0]))
		{
			//printf("himayaaaa> \n");
			joined_path = ft_strjoin(path[i], "/");
			tmp_join = joined_path;
			joined_path = ft_strjoin(joined_path, minishell->cmd[0]);
			//printf("joined_path: %s\n", joined_path);
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
	printf("chyooo???\n");
	if (!minishell->tokens || minishell->tokens->value)
	{
		printf("chyooo\n");
		return (0);
	}
	printf("vyy chyooo\n");

	while (minishell && minishell->tokens)
	{
		if (minishell->tokens->type == S_PIPE)
			pipe_count++;
		minishell->tokens = minishell->tokens->next;
	}
	minishell->tokens = tmp;
	return (pipe_count);
}

int	check_redir(char *str)
{
	if (ft_strcmp(str, "<") == 0
		|| ft_strcmp(str, ">") == 0
		|| ft_strcmp(str, "<<") == 0
		|| ft_strcmp(str, ">>") == 0)
		return (1);
	return (0);
}
char	**clean_cmd(char **cmd)
{
	int	i;

	i = 0;
	while (cmd[i])
	{
		if (ft_strcmp(cmd[i], ">") == 0 || ft_strcmp(cmd[i], ">>") == 0 || ft_strcmp(cmd[i], "<") == 0 || ft_strcmp(cmd[i], "<<") == 0)
		{
			free(cmd[i]);
			if (cmd[i + 1])
				free(cmd[i++]);
			continue ;
		}
		i++;
	}
	return (cmd);
}
void	handle_redirection(t_minishell *minishell, int type, char *file_name) {
	//printf("heredoc?????\n");
	if (type == IN_REDIR)
		minishell->fd_in = open(file_name, O_RDONLY);
	else if (type == APPEND_FILEOUT)
		minishell->fd_out = open(file_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (type == OUT_REDIR)
		minishell->fd_out = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (type == HERE_DOC)
	{
		printf("heredoc\n");
		minishell->fd_heredoc = here_doc_open(file_name);
		return ;
	}
	// if (minishell->fd == -1) {
	// 	perror("open");
	// 	return ;
	// }
}


void	built_in(t_minishell *minishell)
{
	t_split	*tmp;
	int		pipes;
	int		curr;
	int		pid;
	char	**envp;

	curr = 0;
	tmp = minishell->tokens;
	pipes = pipe_count(minishell);
	//pipe
	init_pipe_fd(minishell, pipes);
	printf("helloo builtin\n");
	printf("%d\n", pipes);
	while (minishell->tokens && minishell->tokens->value)
	{
		minishell->tokens = pre_execve(minishell);
		check_cmd(minishell);
		envp = env_to_matrix(minishell);
		printf("cmd[%d] = %s\n", 0, minishell->cmd[0]);
		
		if (curr < pipes + 1)
		{
			if (pipes == 0 && isbuiltin(minishell->cmd[0]) == 1)
			{
				if (ft_strcmp(minishell->cmd[0], "cd") == 0)
				{
					printf("cd is found\n");
					cd(minishell);
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
				}
				else if (ft_strcmp(minishell->cmd[0], "exit") == 0)
				{
					printf("exit is found\n");
					exit_shell(minishell);
				}
				else if (ft_strcmp(minishell->cmd[0], "export") == 0)
				{
					printf("export is found\n");
					export_bulki(minishell, envp);
				}
				if (ft_strcmp(minishell->cmd[0], "echo") == 0)
				{
					printf("echo is found\n");
					echo(minishell->cmd);
				}
				else if (ft_strcmp(minishell->cmd[0], "pwd") == 0)
				{
					printf("pwd is found\n");
					pwd();
				}
			}
			else
			{
				pid = fork();
				//exit(1);
				// if (dup2(minishell->fd, 1) == -1)
				// 	exit(1);
				//printf("%d\n", minishell->fd);
				if (pid == 0)
				{
					ft_dups(minishell, pipes, curr);
					//dup2(minishell->fd, 1);
					//dprintf(2, "aaaa\n");
					printf("tokens: %p\n", minishell->tokens);
					//handle_redirection(minishell->cmd);
					if (execve(minishell->cmd[0], minishell->cmd, envp) == -1) //because execve exits after running the command and we don't need to quit from the main proccess (program) that's why it is put in fork
						exit(1);//error and exit maybe
				}
			}
			curr++;
		}
		printf("here ?? \n");
		waitpid(pid, NULL, 0);
		printf("baaaa here ?? \n");
	
		//continue;
		//minishell->tokens = minishell->tokens->next;
	}
	minishell->tokens = tmp;
}
