#ifndef MINISHELL_H
# define MINISHELL_H

# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>

typedef	struct s_split
{
	char			*value;
	char			*type;
	struct s_split	*next;
}	t_split;

void	tokenization(char *input, char **env);
t_split	*ft_lstnew(char *content, char *type);
void	dollar(t_split	*item, char **env);
char	*ft_substr(char *s, int start, int len);
int		ft_strlen(char *str);
int		check_key_in_env(char **env, char *key);
#endif