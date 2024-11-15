#include "minishell.h"

void	in_redir(t_minishell *minishell)
{
	if (minishell->fd_in > 0)
	{
		if (dup2(minishell->fd_in, 0) == -1)
		{
			close(minishell->fd_in);
			//err_message("minishell: ", "dup2 error\n", "");
			//g_exit_status = 2;
		}
		close(minishell->fd_in);
	}
}

void	out_redir(t_minishell *minishell)
{
	//printf("tpem->%d\n",minishell->fd_out);
	if (minishell->fd_out > 1)
	{
		if (dup2(minishell->fd_out, 1) == -1)
		{
			
			close(minishell->fd_out);
			//err_message("minishell: ", "dup2 error\n", "");
			//g_exit_status = 2;
		}
		close(minishell->fd_out);
	}
}

void	redirs(t_minishell *minishell)
{
	in_redir(minishell);
	out_redir(minishell);
	//printf("end of redirs\n");
}
