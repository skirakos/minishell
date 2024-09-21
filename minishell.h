#ifndef MINISHELL_H
# define MINISHELL_H

# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>

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

} t_minishell;

void	tokenization(char *input, t_env	*env);
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

#endif