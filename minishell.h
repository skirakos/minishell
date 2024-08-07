#ifndef MINISHELL_H
# define MINISHELL_H

# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include <stdio.h>

typedef	struct s_split
{
	char			*value;
	char			*type;
	struct s_split	*next;
}	t_split;

void	tokenization(char *input);

#endif