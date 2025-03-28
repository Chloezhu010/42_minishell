/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czhu <czhu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 18:52:27 by auzou             #+#    #+#             */
/*   Updated: 2025/03/25 11:54:58 by czhu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/sig.h"

static t_env	*g_env = NULL;
/* set global env pointer */

void	set_signal_env(t_env *env)
{
	g_env = env;
}

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

/* reset stdin state */
void reset_input_state(void)
{
	int fd;
	struct termios term;

	/* reset terminal attributes */
	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag |= (ICANON | ECHO);
	term.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);

	/* ensure stdin i properly setup */
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
	if (g_env && g_env->at_prompt)
	{
        write(STDOUT_FILENO, "\n", 1);
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
        g_env->exit_status = 130;
    }
}

/* Ctrl \ handler */
void sigquit_handler(int sig)
{
    (void)sig;
    if (g_env && g_env->at_prompt)
    {
        write(STDOUT_FILENO, "\n", 1);
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
    }
    if (g_env)
        g_env->exit_status = 131;
}
