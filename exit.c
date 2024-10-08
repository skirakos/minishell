#include "minishell.h"

int g_exit_status = 0;

int	is_valid(char	*status)
{
	int	i;

	i = 0;
	while (status[i])
	{
		if (status[i] >= '0' && status[i] <= '9')
			i++;
		else
			return (-1);
	}
	return (0);
}

void	exit_shell(t_minishell *minihell)
{
	if (matrix_len(minihell->cmd) > 2)
	{
		ft_putstr_fd("Minishell gjuk: exit: too many arguments\n", 2);
		g_exit_status = 1;
	}
	else if (minihell->cmd[1] && is_valid(minihell->cmd[1]) == 0)
	{
		g_exit_status = ft_atoi(minihell->cmd[1]);
		if (g_exit_status < 0 || g_exit_status > 255)
        	g_exit_status = g_exit_status % 256; // ba vor negative lini ??? kara?
	}
	else
		g_exit_status = 255;
	free_before_exit(minihell);
	exit(g_exit_status);
}