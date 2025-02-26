#include "../../incl/sig.h"

/* global variable for exit status */
extern int g_exit_status;

/* Ctrl C handler */
void sigint_handler(int sig)
{
    (void)sig;
    write(STDOUT_FILENO, "\n", 1);
    rl_on_new_line(); // tell readline to move to a new line
    rl_replace_line("", 0); // clear the current input line
    rl_redisplay(); 
    // reset the exit status
    g_exit_status = 130;
}

/* Ctrl \ handler */
void sigquit_handler(int sig)
{
    (void)sig;
}

/* setup signals */
void setup_signal(void)
{
    struct sigaction sa;

    // handle SIGINT
    sigemptyset(&sa.sa_mask);
    sa.sa_handler = sigint_handler;
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);
    // ignore SIGQUIT
    sa.sa_handler = sigquit_handler;
    sigaction(SIGQUIT, &sa, NULL);
}
