#ifndef SIMPLESHELL_H
# define SIMPLESHELL_H
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>

/* ANSI color codes for terminal output formatting
    Y - yellow
    G - green
    C - cyan
    RED - red
    RST - reset to default color
*/

# define DEL "\n\t \v\f\r"
# define CELL_Jr 0

typedef struct s_builtin
{
    const char  *builtin_name;
    int (*foo)(char **);
}   t_builtin;

void    Getcwd(char *buf, size_t bufsize);
void    *Malloc(size_t size);
void    *Realloc(void *ptr, size_t size);
pid_t   Fork(void);
void    Execvp(const char *file, char *const av[]);
pid_t   Wait(int *status);
int     cell_exit(char **args);
int     cell_env(char **args);

#endif