#ifndef MINISHELL_H
# define MINISHELL_H

# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>

typedef struct s_env	t_env;

typedef	struct s_split
{
	char			*value;
	char			*type;
	struct s_split	*next;
}	t_split;

typedef	struct s_env
{
	char	*var;
	char	*value;
	t_env	*next;
}	t_env;

void	tokenization(char *input, t_env	*env);
t_split	*ft_lstnew(char *content, char *type);
void	dollar(t_split	*item, t_env *env);
char	*ft_substr(char *s, int start, int len);
int		ft_strlen(char *str);
int		check_key_in_env(t_env *env, char *key, t_split *item);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_strjoin(char *s1, char *s2);

#endif