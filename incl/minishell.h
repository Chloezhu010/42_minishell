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
# include <termios.h>
# include "../src/libft/libft.h"
# include "../incl/execution.h"
# include "../incl/builtin.h"
# include "../incl/loop.h"
# include "../incl/sig.h"

# define CHILD_PROCESS 0

typedef struct s_cmd t_cmd;
typedef struct s_env t_env;

/* pipe */
void execute_pipeline(t_cmd *cmds, t_env *env);
void ft_pipe(int pipefd[2]);

/* shell loop */
char *read_line(void);
char    **cell_split_line(char *line);
void launch_execution(char **args, t_env *env);
void execute_shell(char **args, t_env *env);
void shell_loop(t_env *env);

/* shell loop utils */
void	*ft_malloc(size_t size);
int		ft_strcmp(const char *s1, const char *s2);

#endif
