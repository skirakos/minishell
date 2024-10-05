#include "minishell.h"

int	is_valid(char	*status)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] >= '0' && str[i] <= '9')
			i++;
		else
			return (-1);
	}
	return (0);
}
void	exit_shell(t_minishell *minihell)
{
	int	exit_code;

	if (minihell->cmd[1] && is_valid(minihell->cmd[1]) == 0)
	{
		exit_code = ft_atoi(minihell->cmd[1]);
		if (exit_code < 0 || exit_code > 255)
        	exit_code = exit_code % 256;
		exit(exit_code);
	}
}