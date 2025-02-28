#ifndef EXECUTION_H
# define EXECUTION_H
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include "../src/libft/libft.h"
# include "../incl/builtin.h"

typedef struct s_env t_env;

/* execute_path */
char *find_path(char *cmd);

/* execute_utils */
pid_t   Fork(void);
pid_t   Wait(int *status);
void Execve(char *path, char **av, t_env *env);

#endif