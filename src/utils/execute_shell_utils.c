/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_shell_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czhu <czhu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 18:01:10 by auzou             #+#    #+#             */
/*   Updated: 2025/03/25 11:55:27 by czhu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

void	execute_builtin(t_cmd *cmd, t_env *env)
{
	t_builtin	*builtins;
	int			i;

	builtins = init_builtin();
	i = 0;
	while (builtins[i].builtin_name)
	{
		if (cmd->args && ft_strcmp(cmd->args[0], builtins[i].builtin_name) == 0)
		{
			builtins[i].func(cmd->args, env);
			return ;
		}
		i++;
	}
}

void	cleanup_external(t_cmd *cmd_head, t_env *env, t_fd *fd)
{
	if (fd->stdin != -1)
		close(fd->stdin);
	if (fd->stdout != -1)
		close(fd->stdout);
	free_cmds(cmd_head);
	free_env(env);
	exit(env->exit_status);
}

void	execute_external(t_cmd *cmd_head, t_cmd *cmd, t_env *env, t_fd *fd)
{
	char	*path;

	path = get_cmd_path(cmd, env);
	if (path)
	{
		ft_execve(path, cmd->args, env);
		free(path);
		if (env->exit_status != 0)
			cleanup_external(cmd_head, env, fd);
	}
	if (cmd->args && cmd->args[0])
		ft_putstr_fd(" command not found\n", 2);
	exit_status(env, 127);
	cleanup_external(cmd_head, env, fd);
}

int	is_builtin(char *cmd)
{
	char	*builtins[8];
	int		i;

	builtins[0] = "cd";
	builtins[1] = "echo";
	builtins[2] = "env";
	builtins[3] = "exit";
	builtins[4] = "export";
	builtins[5] = "pwd";
	builtins[6] = "unset";
	builtins[7] = NULL;
	i = 0;
	while (builtins[i])
	{
		if (ft_strcmp(cmd, builtins[i]) == 0)
			return (1);
		i++;
	}
	return (0);
}

void	execute_cmd(t_cmd *cmd_head, t_cmd *cmd, t_env *env, t_fd *fd)
{
	if (is_builtin(cmd->args[0]))
	{
		execute_builtin(cmd, env);
		return ;
	}
	execute_external(cmd_head, cmd, env, fd);
}
