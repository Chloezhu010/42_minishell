#ifndef SIG_H
# define SIG_H
# include "../incl/minishell.h"

void sigint_handler(int sig);
void sigquit_handler(int sig);
void setup_signal(void);

#endif