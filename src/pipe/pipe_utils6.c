/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils6.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czhu <czhu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 19:25:46 by czhu              #+#    #+#             */
/*   Updated: 2025/04/04 19:29:25 by czhu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/pipe.h"
#include "../../incl/minishell.h"

/* helper function for execute_pipeline
	- setup pipeline
*/
void	setup_pipeline(t_pipe *ctx, t_cmd *cmd,
	t_env *env, struct sigaction old[2])
{
	init_pipe(ctx);
	mark_pipeline_cmd(cmd);
	save_signal_handlers(&old[0], &old[1]);
	set_parent_signals_for_pipeline();
	env->at_prompt = 0;
}
