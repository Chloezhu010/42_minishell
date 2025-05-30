/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czhu <czhu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 18:52:27 by auzou             #+#    #+#             */
/*   Updated: 2025/03/29 15:01:08 by czhu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/sig.h"

t_env	*g_env;

void	set_signal_env(t_env *env)
{
	g_env = env;
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
		exit_status(g_env, 130);
	}
}

/* Ctrl \ handler */
void	sigquit_handler(int sig)
{
	(void)sig;
	if (g_env && g_env->at_prompt)
	{
		rl_on_new_line();
		rl_redisplay();
		exit_status(g_env, 0);
		return ;
	}
	else if (g_env)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		exit_status(g_env, 131);
	}
}

/* Ctrl C handler for heredoc */
void	heredoc_sigint_handler(int sig)
{
	if (sig == SIGINT && g_env)
	{
		g_env->heredoc_interrupted = 1;
		ioctl(STDIN_FILENO, TIOCSTI, "\n");
		write(STDERR_FILENO, "\n", 1);
		exit_status(g_env, 130);
	}
}
