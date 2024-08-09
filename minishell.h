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

void	tokenization(char *input);
t_split	*ft_lstnew(char *content, char *type);

#endif