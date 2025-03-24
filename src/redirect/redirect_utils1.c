/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_utils1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czhu <czhu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 18:36:51 by auzou             #+#    #+#             */
/*   Updated: 2025/03/23 16:50:41 by czhu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

/*
    - create a temp file to store input line
        - write only
        - if not exits, create one
        - if the file exists, content will be erased
    - read user input until the delimiter
        - input lines are read & written to the temp file
        - close and reopen with read-only mode
    - reopen the file and return fd
*/
int	handle_heredoc(char *delimiter, t_env *env)
{
	char	*line;
	char	*expanded_line;
	int		fd;

	fd = open("/tmp/minishell_heredoc", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("open");
		env->exit_status = 1;
		return (-1);
	}
	while (1)
	{
		line = readline("heredoc> ");
		if (!line || ft_strcmp(line, delimiter) == 0)
			break ;
		expanded_line = expand_var_instr(line, env);
		write(fd, expanded_line, ft_strlen(expanded_line));
		write(fd, "\n", 1);
		free(expanded_line);
		free(line);
	}
	return (fd);
}

int	handle_heredoc_redirect(t_cmd *cmd, int *stdin_backup, t_env *env)
{
	if (dup2(cmd->fd_in, STDIN_FILENO) == -1)
	{
		perror("dup2");
		close(*stdin_backup);
		env->exit_status = 1;
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
		env->exit_status = 1;
		return (-1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("dup2");
		close(fd);
		close(*stdin_backup);
		env->exit_status = 1;
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
		env->exit_status = 1;
		return (-1);
	}
	if (cmd->heredoc && cmd->fd_in > 0)
		return (handle_heredoc_redirect(cmd, stdin_backup, env));
	if (cmd->infile)
		return (handle_file_input_redirect(cmd, stdin_backup, env));
	return (0);
}

/* handle output redirect TODO */
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
