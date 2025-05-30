/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_utils1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czhu <czhu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 18:36:51 by auzou             #+#    #+#             */
/*   Updated: 2025/03/29 14:23:40 by czhu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

int	handle_heredoc_redirect(t_cmd *cmd, int *stdin_backup, t_env *env)
{
	if (dup2(cmd->fd_in, STDIN_FILENO) == -1)
	{
		perror("dup2");
		close(*stdin_backup);
		exit_status(env, 1);
		return (-1);
	}
	close(cmd->fd_in);
	return (0);
}

int	handle_file_input_redirect(t_cmd *cmd, int *stdin_backup, t_env *env)
{
	int	fd;

	fd = open(cmd->infile, O_RDONLY);
	if (fd == -1)
	{
		perror("minishell");
		close(*stdin_backup);
		exit_status(env, 1);
		return (-1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("dup2");
		close(fd);
		close(*stdin_backup);
		exit_status(env, 1);
		return (-1);
	}
	close(fd);
	return (0);
}

int	handle_input_redirect(t_cmd *cmd, int *stdin_backup, t_env *env)
{
	if (!cmd->heredoc && !cmd->infile)
	{
		*stdin_backup = -1;
		return (0);
	}
	*stdin_backup = dup(STDIN_FILENO);
	if (*stdin_backup == -1)
	{
		perror("dup");
		exit_status(env, 1);
		return (-1);
	}
	if (cmd->heredoc && cmd->fd_in > 0)
		return (handle_heredoc_redirect(cmd, stdin_backup, env));
	if (cmd->infile)
		return (handle_file_input_redirect(cmd, stdin_backup, env));
	return (0);
}
