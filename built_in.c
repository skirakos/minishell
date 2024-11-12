#include "minishell.h"

t_split *pre_execve(t_minishell *minishell)
{
	t_split *tmp;
	t_split *ptr;
	int i;
	int len;
	char **matrix;

	ptr = minishell->tokens;
	tmp = minishell->tokens;
	len = 0;
	while (minishell->tokens && minishell->tokens->value && (minishell->tokens->type == WORD || minishell->tokens->type != S_PIPE))
	{
		if (minishell->tokens->type != WORD && check_redir(minishell->tokens->value))
			len -= 2;
		len++;
		minishell->tokens = minishell->tokens->next;
		// printf("tokens->%p\n",minishell->tokens);
	}
	matrix = (char **)malloc(sizeof(char *) * (len + 1));
	if (!matrix)
		return (NULL);
	i = 0;
	while (tmp && tmp->value && tmp->type != S_PIPE)
	{
		if (tmp->type == WORD || !check_redir(tmp->value))
		{
			matrix[i] = ft_strdup(tmp->value);
			i++;
		}
		else
		{
			handle_redirection(minishell, tmp->type, tmp->next->value);
			tmp = tmp->next;
		}
		tmp = tmp->next;
	}
	matrix[len] = NULL;
	
	// i = 0;
	// while (matrix[i] != NULL)
	// {
	// 	printf("matrix[%d] %s\n", i, matrix[i]);
	// 	i++;
	// }
	minishell->cmd = matrix;
	if (tmp && tmp->type == S_PIPE)
		minishell->tokens = minishell->tokens->next;
	// while(ptr != minishell->tokens)
	// {
	// 	tmp = ptr;
	// 	free(ptr->value);
	// 	ptr = ptr->next;
	// 	free(tmp);
	// }
	
	return (minishell->tokens);
}

int search_slash(char *str)
{
	int i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '/')
			return (0);
		i++;
	}
	return (-1);
}

char **env_to_matrix(t_minishell *minishell)
{
	int len;
	t_env *env;
	char **envp;
	char *join_tmp;
	char *zibil;
	int i;

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

void	fooo()
{
	// system("leaks minishell");
}

int is_builtin(char *cmd)
{
	if (ft_strcmp(cmd, "echo") == 0 || ft_strcmp(cmd, "cd") == 0 || ft_strcmp(cmd, "pwd") == 0 || ft_strcmp(cmd, "export") == 0 || ft_strcmp(cmd, "unset") == 0 || ft_strcmp(cmd, "env") == 0 || ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}
void check_cmd(t_minishell *minishell)
{
	t_env *tmp;
	char **path;
	char *joined_path;
	char *tmp_join;
	int i;

	if (!minishell->cmd[0])
	{
		return;
	}
	tmp = minishell->env;
	if ((minishell->cmd[0][0] == '.' && minishell->cmd[0][1] == '/') || search_slash(minishell->cmd[0]) == 0)
	{
		if (access(minishell->cmd[0], X_OK) == 0)
			return;
		else if (minishell->cmd[0][0] == '.' && minishell->cmd[0][1] == '/')
			return;
	}
	if (minishell->cmd[0][0] == '.' && minishell->cmd[0][1] == '/')
	{
		if (access(minishell->cmd[0], X_OK) != 0)
			return;
	}
	path = NULL;
	while (minishell->env)
	{
		if (ft_strcmp(minishell->env->var, "PATH") == 0)
			path = ft_split(minishell->env->value, ':');
		minishell->env = minishell->env->next;
	}
	i = 0;
	while (path && path[i])
	{
		if (!is_builtin(minishell->cmd[0]))
		{
			joined_path = ft_strjoin(path[i], "/");
			tmp_join = joined_path;
			joined_path = ft_strjoin(joined_path, minishell->cmd[0]);
			free(tmp_join);
			if (access(joined_path, X_OK) == 0)
			{
				free(minishell->cmd[0]);
				minishell->cmd[0] = ft_strdup(joined_path);
				free(joined_path);
				free_matrix(path, matrix_len(path));
				minishell->env = tmp;
				return;
			}
			free(joined_path);
		}
		i++;
	}
	minishell->env = tmp;
	if (path)
		free_matrix(path, matrix_len(path));
}
int pipe_count(t_minishell *minishell)
{
	int pipe_count;
	t_split *tmp;
	tmp = minishell->tokens;
	pipe_count = 0;
	if (!minishell->tokens || !minishell->tokens->value)
	{
		printf("return \n");
		return (0);
	}
	while (minishell && minishell->tokens)
	{
		if (minishell->tokens->type == S_PIPE)
			pipe_count++;
		minishell->tokens = minishell->tokens->next;
	}
	minishell->tokens = tmp;
	return (pipe_count);
}

int check_redir(char *str)
{
	if (ft_strcmp(str, "<") == 0 || ft_strcmp(str, ">") == 0 || ft_strcmp(str, "<<") == 0 || ft_strcmp(str, ">>") == 0)
		return (1);
	return (0);
}

char **clean_cmd(char **cmd)
{
	int i;

	i = 0;
	while (cmd[i])
	{
		if (ft_strcmp(cmd[i], ">") == 0 || ft_strcmp(cmd[i], ">>") == 0 || ft_strcmp(cmd[i], "<") == 0 || ft_strcmp(cmd[i], "<<") == 0)
		{
			free(cmd[i]);
			if (cmd[i + 1])
				free(cmd[i++]);
			continue;
		}
		i++;
	}
	return (cmd);
}

void handle_redirection(t_minishell *minishell, int type, char *file_name)
{
	if (type == IN_REDIR)
		minishell->fd_in = open(file_name, O_RDONLY);
	else if (type == APPEND_REDIR)
		minishell->fd_out = open(file_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (type == OUT_REDIR)
		minishell->fd_out = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (type == HERE_DOC)
	{
		minishell->fd_heredoc = here_doc_open(file_name);
		return;
	}
}

void	wait_processes(t_minishell *minishell, int pipes) // to wait every child proccess of each main proccess
{
	int		i;
	int		exit_status;

	i = 0;
	if (pipes == 0 && isbuiltin(minishell->cmd[0]) == 1)
		return ;
	while (i < pipes + 1)
	{
		waitpid(minishell->pid[i], &exit_status, 0);
		if (WIFEXITED(exit_status))
			g_exit_status = WEXITSTATUS(exit_status);
		else if (WIFSIGNALED(exit_status))
			g_exit_status = 128 + WTERMSIG(exit_status);
		i++;
	}
}

void built_in(t_minishell *minishell)
{
	t_split *tmp;
	int pipes;
	int curr;
	int pid;
	char **envp;

	curr = 0;
	tmp = minishell->tokens;
	pipes = pipe_count(minishell);
	minishell->pid = malloc(sizeof(pid_t) * (pipes + 1));
	init_pipe_fd(minishell, pipes);
	while (minishell->tokens && minishell->tokens->value)
	{
		minishell->tokens = pre_execve(minishell);
		check_cmd(minishell);
		envp = env_to_matrix(minishell);
		fooo();
		if (curr < pipes + 1)
		{
			if (pipes == 0 && isbuiltin(minishell->cmd[0]) == 1)
			{
				if (ft_strcmp(minishell->cmd[0], "cd") == 0)
				{
					cd(minishell);
				}
				else if (ft_strcmp(minishell->cmd[0], "env") == 0)
				{
					env(minishell);
				}
				else if (ft_strcmp(minishell->cmd[0], "unset") == 0)
				{
	
					unset(minishell);
				}
				else if (ft_strcmp(minishell->cmd[0], "exit") == 0)
				{

					exit_shell(minishell);
				}
				else if (ft_strcmp(minishell->cmd[0], "export") == 0)
				{
		
					export_bulki(minishell, envp);
				}
			}
			else
			{
				pid = fork();
				if (pid == -1 && g_exit_status != 1)
				{
					g_exit_status = 1;
					perror_exit(minishell, FORK_ERR, "Resource temporarily unavailable", 0);
				}
				else if (pid == 0)
				{
					ft_dups(minishell, pipes, curr);
					redirs(minishell);
					if (access(minishell->cmd[0], X_OK) != 0 && !is_builtin(minishell->cmd[0]))
					{
						perror_exit(minishell, CMD_NOT_FOUND,minishell->cmd[0], 1);
						exit (127);
					}
					else if (ft_strcmp(minishell->cmd[0], "cd") == 0)
					{
						cd(minishell);
						exit(0);
					}
					else if (ft_strcmp(minishell->cmd[0], "env") == 0)
					{
						env(minishell);
						exit(0);
					}
					else if (ft_strcmp(minishell->cmd[0], "unset") == 0)
					{
						unset(minishell);
						exit(0);
					}
					else if (ft_strcmp(minishell->cmd[0], "exit") == 0)
					{
						exit_shell(minishell);
					}
					else if (ft_strcmp(minishell->cmd[0], "export") == 0)
					{
						export_bulki(minishell, envp);
						exit(0);
					}
					else if (ft_strcmp(minishell->cmd[0], "echo") == 0)
					{
						echo(minishell->cmd);
						exit(0);
					}
					else if (ft_strcmp(minishell->cmd[0], "pwd") == 0)
					{
						pwd();
						exit(0);
					}
					else if (ft_strcmp(minishell->cmd[0], "env") == 0)
					{
						env(minishell);
						exit(0);
					}
					else if (execve(minishell->cmd[0], minishell->cmd, envp) == -1)
					{
						perror_exit(minishell, EXECVE_ERR, minishell->cmd[0], 1);
						exit(1); // error and exit maybe
					} // because execve exits after running the command and we don't need to quit from the main proccess (program) that's why it is put in fork
				}
				else
					minishell->pid[curr] = pid;
			}
			curr++;
		}
		wait_processes(minishell, pipes);
		minishell->cmd = free_matrix(minishell->cmd, matrix_len(minishell->cmd));
		envp = free_matrix(envp, matrix_len(envp));
		// fooo();
	}
	close_fd(minishell,pipes);
	if (minishell->fd_in != 0)
	{
		close(minishell->fd_in);
		minishell->fd_in = 0;
	}
	if (minishell->fd_out != 1)
	{
		close(minishell->fd_out);
		minishell->fd_out = 1;
	}
	minishell->tokens = tmp;
	while (minishell->tokens)
	{
		tmp = minishell->tokens;
		free (minishell->tokens->value);
		minishell->tokens = minishell->tokens->next;
		free(tmp);
	}
	free(minishell->pid);
	minishell->pid = NULL;
	free(minishell->fd);
	minishell->fd = NULL;
}

