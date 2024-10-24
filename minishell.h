#ifndef MINISHELL_H
# define MINISHELL_H

# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <fcntl.h>
# include <limits.h>

extern int g_exit_status;

typedef struct s_env		t_env;
typedef struct s_split		t_split;
typedef struct s_minishell	t_minishell;
typedef enum e_token_type {
    WORD,
    S_PIPE,
    D_PIPE,
    S_AND,
    D_AND,
    D_QUOTE,
    S_QUOTE,
    IN_REDIR,
    OUT_REDIR,
    HERE_DOC,
    APPEND_REDIR,
    ERROR,
    NONE,
    FILEIN,
    LIMITER,
    FILEOUT,
    APPEND_FILEOUT
} t_token_type;

typedef struct s_split {
    char 			*value;
    t_token_type	type;
    struct s_split	*next;
} t_split;

typedef	struct s_env
{
	char	*var;
	char	*value;
	t_env	*next;
}	t_env;

typedef	struct s_minishell {
	t_split	*tokens;
	char	**cmd;
	t_env	*env;
    int     (*fd)[2];
    int     fd_in;
    int     fd_out;
    int     fd_heredoc;
} t_minishell;

void	tokenization(char *input, t_minishell *minishell);
t_split	*ft_lstnew(char *content);
void	dollar(t_split	*item, t_env *env);
char	*ft_substr(char *s, int start, int len);
int		ft_strlen(char *str);
int		check_key_in_env(t_env *env, char *key, t_split *item);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_strjoin(char *s1, char *s2);
void	ft_strcpy(char *dst, char *src);
char	*ft_strdup(char *s1);
int		ft_strcmp(const char *s1, const char *s2);
void	built_in(t_minishell *minishell);
int		matrix_len(char **matrix);
void	*free_matrix(char** matrix, int rows);
char	**ft_split(char *str, char c);
void	echo(char **args);
void	cd(t_minishell *minishell);
void	ft_putstr_fd(char const *s, int fd);
void	pwd();
void	env(t_minishell *minishell);
void	unset(t_minishell *minishell);
void	exit_shell(t_minishell *minihell);
int		ft_atoi(char *str);
void	free_before_exit(t_minishell *minihell);
void	free_t_env(t_env *env);
void	free_t_split(t_split *tokens);
void	export_bulki(t_minishell *minishell, char **envp);
void	merge_sort(char **arr, int left, int right);
int     here_doc_open(char *limiter);
int     isbuiltin(char *cmd_name);
void	handle_redirection(t_minishell *minishell, int type, char *file_name);
int     check_redir(char *str);
int     init_pipe_fd(t_minishell *minishell, int pipe_count);
void    ft_dups(t_minishell *minishell, int pipes, int curr);
void	redirs(t_minishell *minishell);
void	close_fd(t_minishell *minishell, int pipe_count);

#endif