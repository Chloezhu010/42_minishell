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

/* helpder function for handle_heredoc */
static int	create_heredoc_file(t_env *env)
{
	int	fd;

	fd = open("/tmp/minishell_heredoc", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("open");
		exit_status(env, 1);
		return (-1);
	}
	return (fd);
}

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
int	handle_heredoc(char *delimiter, t_env *env, int expand_var)
{
	char	*line;
	char	*expanded_line;
	int		fd;

	fd = create_heredoc_file(env);
	if (fd == -1)
		return (-1);
	setup_heredoc_signals(env);
	env->heredoc_interrupted = 0;
	while (!env->heredoc_interrupted)
	{
		line = readline("heredoc> ");
		if (!line)
		{
			if (!env->heredoc_interrupted)
				ft_putstr_fd("heredoc delimited by eof\n", STDERR_FILENO);
			break ;
		}
		if (env->heredoc_interrupted)
		{
			free(line);
			break ;
		}
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		if (expand_var)
			expanded_line = expand_var_instr(line, env);
		else
			expanded_line = ft_strdup(line);
		if (expanded_line)
		{
			write(fd, expanded_line, ft_strlen(expanded_line));
			free(expanded_line);
		}
		write(fd, "\n", 1);
		free(line);
	}
	close(fd);
	if (env->heredoc_interrupted)
	{
		unlink("/tmp/minishell_heredoc");
		return (-1);
	}
	fd = open("/tmp/minishell_heredoc", O_RDONLY);
	if (fd == -1)
	{
		perror("open");
		exit_status(env, 1);
		return (-1);
	}
	setup_signal(env);
	return (fd);
}

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
