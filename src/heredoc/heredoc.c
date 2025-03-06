#include "../../incl/minishell.h"

/*
    - create a temp file to store input line
    - read user input until the delimiter
    - redirect input from temp file
    - cleanup temp file
    - return the fd
*/
int handle_heredoc(char *delimiter)
{
    char *line;
    int fd;
    
    /* create a temp file */
    fd = open("/tmp/minishell_heredoc", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
    {
        perror("open");
        return (-1);
    }
    /* read input until the delimiter
        redirect input from temp file
        cleanup temp file
        close file
    */
    while (1)
    {
        line = readline("heredoc> ");
        if (!line || ft_strcmp(line, delimiter) == 0)
            break ;
        write(fd, line, ft_strlen(line));
        write(fd, "\n", 1);
        free(line);
    }
    close(fd);
    /* reopen the file */
    fd = open("/tmp/minishell_heredoc", O_RDONLY);
    return (fd);
}