#ifndef SIG_H
# define SIG_H
# include "../incl/minishell.h"

void set_signal_env(t_env *env);
void	disable_echo(void);
void	enable_echo(void);
void	sigint_handler(int sig);
void	sigquit_handler(int sig);
void	setup_signal(t_env *env);

#endif
