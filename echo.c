#include "minishell.h"

int	check_newline(char	*str)
{
	int	i;

	if (str && str[0] == '-' && str[1] != '\0')
	{
		i = 1;
		while (str[i] == 'n')
			i++;
		if (str[i] == '\0')
			return (0);
	}
	
	return (-1);
}

void	echo(char **args)
{
	int	newline;
	int	i;

	i = 1;
	newline = 0;
	printf("hi from echo\n");
	printf("cmd[%d] = %s\n", i, args[i]);
	while (check_newline(args[i]) == 0)
	{
		newline = 1;
		i++;
	}
	while (args[i])
	{
		write(STDOUT_FILENO, args[i], ft_strlen(args[i]));
		if (args[i + 1])
            write(STDOUT_FILENO, " ", 1);
		i++;
	}
	if (!newline) {
        write(STDOUT_FILENO, "\n", 1);
    }
	
}