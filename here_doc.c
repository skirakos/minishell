#include "minishell.h"

void    here_doc_write(int fd, char *limiter)
{
    char    *input;

    input = NULL;
    while (1)
    {
        input = readline("");
        if (ft_strcmp(input, limiter) == 0)
        {
            free(input);
            break;
        }
        write(fd, input, ft_strlen(input));
        write(fd, "\n", 1);
        free(input);
    }
    close(fd);
}

int here_doc_open(char *limiter)
{
    int fd;

    fd = open(".heredocfile", O_WRONLY | O_CREAT | O_TRUNC, 0644); //hidden file starts with .
    if (fd == -1)
		return (-1);
    here_doc_write(fd, limiter);
    //close(fd);
    fd = open(".heredocfile", O_RDONLY);
    if (fd == -1)
        return (-1);
    return (fd);
}
