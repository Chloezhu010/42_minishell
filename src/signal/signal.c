#include "../../incl/sig.h"

/* global variable for exit status */
extern int g_exit_status;

/* Ctrl C handler
    - print a new line
    - tell readline to move to a new line
    - clear the current input line
    - re display the prompt
*/
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
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}

/* setup signals */
void setup_signal(void)
{
    struct sigaction sa;
    // struct termios term;

    // config terminal settings
    // tcgetattr(STDIN_FILENO, &term); // get the current terminal attributes
    // term.c_lflag &= ~ECHO; // disable ECHO flag, c_lflag is local modes
    // tcsetattr(STDIN_FILENO, TCSANOW, &term); // set new terminal attributes, TSCANOW apply the changes now

    // handle SIGINT
    sigemptyset(&sa.sa_mask);
    sa.sa_handler = sigint_handler;
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);
    // ignore SIGQUIT
    sa.sa_handler = sigquit_handler;
    sigaction(SIGQUIT, &sa, NULL);
}
