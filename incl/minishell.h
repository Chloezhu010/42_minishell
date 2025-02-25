#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <signal.h>
# include <errno.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <sys/ioctl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <dirent.h>
# include "../src/libft/libft.h"
# include "../incl/execution.h"
# include "../incl/builtin.h"
# include "../incl/loop.h"

# define DEL "\n\t \v\f\r"
# define CHILD_PROCESS 0

/* shell loop utils */
void    *Malloc(size_t size);
void    *Realloc(void *ptr, size_t size);
void    Execvp(const char *file, char *const av[]);
pid_t   Fork(void);
pid_t   Wait(int *status);

#endif