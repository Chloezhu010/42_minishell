/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czhu <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 13:16:34 by czhu              #+#    #+#             */
/*   Updated: 2025/03/02 13:17:57 by czhu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/sig.h"

/* global variable for exit status */
extern int	g_exit_status;

/* disable the print ouf of Ctrl C or Ctrl backslash 
    - using termios.h
    - get the current terminal attributes
    - disable ECHOCTL flag
    - set new terminal attributes: TSCANOW apply the changes now
*/
void	disable_echo(void)
{
	struct termios	term;

	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

/* re-enable echo afterwards */
void	enable_echo(void)
{
	struct termios	term;

	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag |= ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

/* Ctrl C handler
    - print a new line
    - tell readline to move to a new line
    - clear the current input line
    - re display the prompt
    - reset the exit status: 130 for SIGINT
*/
void	sigint_handler(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	g_exit_status = 130;
}

/* Ctrl \ handler */
void	sigquit_handler(int sig)
{
	(void)sig;
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	g_exit_status = 131;
}

/* setup signals
    - disable echo
    - handle SIGINT
    - handle SIGQUIT
*/
void	setup_signal(void)
{
	struct sigaction	sa;

	disable_echo();
	sigemptyset(&sa.sa_mask);
	sa.sa_handler = sigint_handler;
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sigemptyset(&sa.sa_mask);
	sa.sa_handler = sigquit_handler;
	sa.sa_flags = 0;
	sigaction(SIGQUIT, &sa, NULL);
}
