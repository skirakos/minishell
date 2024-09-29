#include "minishell.h"

// void	tilda(t_split *minishell)
// {
// 	char	*path;

// 	path = malloc(ft_strlen(minishell->cmd[1]) + ft_strlen(getenv("HOME")));

// }

void	cd(t_minishell *minishell)
{
	if (!minishell->cmd[1] || ft_strcmp(minishell->cmd[1], "~") == 0 || ft_strcmp(minishell->cmd[1], "~/") == 0)
	{
		chdir(getenv("HOME"));
	}
	// else if (minishell->cmd[1][0] == '~')
	// {
	// 	tilda(minishell);
	// }
}