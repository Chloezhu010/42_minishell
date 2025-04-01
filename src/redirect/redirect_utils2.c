/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auzou <auzou@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 18:38:43 by auzou             #+#    #+#             */
/*   Updated: 2025/03/21 18:42:10 by auzou            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

/* handle output redirect */
int	open_output_file(t_cmd *cmd, t_env *env)
{
	int	fd;

	if (cmd->append)
		fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("minishell");
		env->exit_status = 1;
	}
	return (fd);
}

int	handle_output_redirect(t_cmd *cmd, int *stdout_backup, t_env *env)
{
	int	fd;

	if (!cmd->outfile)
	{
		*stdout_backup = -1;
		return (0);
	}
	*stdout_backup = dup(STDOUT_FILENO);
	if (*stdout_backup == -1)
	{
		perror("dup");
		env->exit_status = 1;
		return (-1);
	}
	fd = open_output_file(cmd, env);
	if (fd == -1)
		return (-1);
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		handle_redirect_error1(fd, stdout_backup, env);
		return (-1);
	}
	close(fd);
	if (*stdout_backup != -1)
	{
		close(*stdout_backup);
		*stdout_backup = -1;
	}
	return (0);
}

/* restore std input after execution */
void	restore_io(int stdin_backup, int stdout_backup)
{
	if (stdin_backup != -1)
	{
		dup2(stdin_backup, STDIN_FILENO);
		close(stdin_backup);
		stdin_backup = -1;
	}
	if (stdout_backup != -1)
	{
		dup2(stdout_backup, STDOUT_FILENO);
		close(stdout_backup);
		stdout_backup = -1;
	}
}
