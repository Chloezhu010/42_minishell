/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils5.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czhu <czhu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 19:20:56 by czhu              #+#    #+#             */
/*   Updated: 2025/04/04 19:29:11 by czhu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/pipe.h"
#include "../../incl/minishell.h"

/* helper function for fork_and_execute_pipe */
void	handle_fork_error(t_pipe *ctx, t_env *env)
{
	perror("fork");
	exit_status(env, 1);
	close_all_pipe_fds(ctx);
	restore_std_fd(ctx);
}

/* main pipeline execution
    - init pipeline ctx and mark cmds
    - check redirect before creating pipes
    - create pipe if not the last cmd
    - fork process
    - restore std fd
    - wait for all child process
*/
void	fork_and_execute_pipe(t_cmd *cmd_head, t_cmd *current,
		t_pipe *ctx, t_env *env)
{
	pid_t	pid;
	int		redirect_error;

	redirect_error = process_redirect(current, env);
	if (current->next)
	{
		if (create_cmd_pipe(ctx, env))
			return ;
	}
	pid = fork();
	if (pid == -1)
	{
		handle_fork_error(ctx, env);
		return ;
	}
	if (pid == CHILD_PROCESS)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		execute_pipe_child(cmd_head, current, ctx, env);
	}
	else
		execute_parent_process(ctx, current, pid);
}

/* Save the original signal handlers before changing them */
void	save_signal_handlers(struct sigaction *old_int,
			struct sigaction *old_quit)
{
	sigaction(SIGINT, NULL, old_int);
	sigaction(SIGQUIT, NULL, old_quit);
}

/* Set parent process signal handling during pipeline execution */
void	set_parent_signals_for_pipeline(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_handler = SIG_IGN;
    sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}

/* Restore original signal handlers */
void	restore_signal_handlers(struct sigaction *old_int,
			struct sigaction *old_quit)
{
	sigaction(SIGINT, old_int, NULL);
	sigaction(SIGQUIT, old_quit, NULL);
}
