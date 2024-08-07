#include "minishell.h"

int main(int argc, char **argv)
{
	char	*input = NULL;

	(void)argv;
	if (argc == 1)
	{
		while (1)
		{
			input = readline("MINISHELL GJUK: ");
			if (input == '\0')
			{
				free(input);
				break;
			}
			add_history(input);
			tokenization(input);
			free(input);
		}
	}
	free(input);
	printf("ahjksd\n");
	//system("leaks minishell");
	return (0);
}