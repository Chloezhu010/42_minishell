/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auzou <auzou@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 13:16:34 by czhu              #+#    #+#             */
/*   Updated: 2025/03/21 18:53:04 by auzou            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/sig.h"

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

/* reset stdin state
	- reset terminal attributes
	- ensure stdin i properly setup
*/
void	reset_input_state(void)
{
	int				fd;
	struct termios	term;

	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag |= (ICANON | ECHO);
	term.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
	if (isatty(STDIN_FILENO))
	{
		fd = open("/dev/tty", O_RDONLY);
		if (fd != STDIN_FILENO && fd > 0)
		{
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
	}
}

/* setup signals
    - disable echo
    - handle SIGINT
    - handle SIGQUIT
*/
void	setup_signal(t_env *env)
{
	struct sigaction	sa;

	set_signal_env(env);
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
