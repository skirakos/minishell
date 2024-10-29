#include "minishell.h"

int	syntax_check(t_split *tokens)
{
	while (tokens != NULL)
	{
		if (tokens->type == OUT_REDIR || tokens->type == IN_REDIR \
			|| tokens->type == APPEND_REDIR || tokens->type == HERE_DOC)
		{
			if (tokens->next == NULL)
				return (perror_exit(SYNTAX_ERR, NULL, tokens->value, 2), 2);
			else if (tokens->next->type == FILEOUT || tokens->next->type == FILEIN \
				|| tokens->next->type == APPEND_FILEOUT \
				|| tokens->next->type == LIMITER)
				return (0);
			perror_exit(SYNTAX_ERR, NULL, tokens->value, 2);
			return (2);
		}
		// else if (tokens->type == S_PIPE && !tokens->prev)
		// 	return (perror_exit(SYNTAX_ERR, NULL, tokens->value, 2), 2);
		else if (tokens->type == S_PIPE && !tokens->next)
			return (perror_exit(SYNTAX_ERR, NULL, tokens->value, 2), 2);
		else if (tokens->type == S_PIPE && is_ctrl_op(tokens->next))
			return (perror_exit(SYNTAX_ERR, NULL, tokens->value, 2), 2);
		else if (is_permited(tokens))
			return (perror_exit(SYNTAX_ERR, NULL, tokens->value, 2), 2);
		tokens = tokens->next;
	}
	return (0);
}