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

/* shell loop utils */
void	*Malloc(size_t size);
int		ft_strcmp(const char *s1, const char *s2);

#endif
