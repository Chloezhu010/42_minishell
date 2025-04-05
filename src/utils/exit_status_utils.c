/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_status_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auzou <auzou@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 19:34:10 by auzou             #+#    #+#             */
/*   Updated: 2025/03/31 19:39:26 by auzou            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

void	exit_status(t_env *env, int exit_status)
{
	if (!env->exit)
	{
		env->exit_status = exit_status;
		env->exit = 1;
	}
}

int	execute_builtin1(t_cmd *cmd, t_env *env,
	int stdin_backup, int stdout_backup)
{
	t_builtin	*builtins;
	int			i;

	builtins = init_builtin();
	i = 0;
	while (builtins[i].builtin_name)
	{
		if (ft_strcmp(cmd->args[0], builtins[i].builtin_name) == 0)
		{
			builtins[i].func(cmd->args, env);
			restore_io(stdin_backup, stdout_backup);
			return (1);
		}
		i++;
	}
	return (0);
}
