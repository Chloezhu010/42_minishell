#ifndef SIG_H
# define SIG_H
# include "../incl/minishell.h"

void	disable_echo(void);
void	enable_echo(void);
void	sigint_handler(int sig);
void	sigquit_handler(int sig);
void	setup_signal(void);

#endif
