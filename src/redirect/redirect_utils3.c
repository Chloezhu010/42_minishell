/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_utils3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czhu <czhu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 15:05:51 by czhu              #+#    #+#             */
/*   Updated: 2025/04/05 15:09:26 by czhu             ###   ########.fr       */
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

/* helper function for handle_heredoc */
static void	write_line_to_heredoc(char *line, int fd,
	t_env *env, int expand_var)
{
	char	*expanded_line;

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

/* helper function for handle_heredoc */
static int	cleanup_heredoc(int fd, t_env *env)
{
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

/* helper function for handle_heredoc */
static int	process_heredoc_line(char *line, char *delimiter, t_env *env)
{
	if (!line)
	{
		if (!env->heredoc_interrupted)
			ft_putstr_fd("heredoc delimited by eof\n", STDERR_FILENO);
		return (1);
	}
	if (env->heredoc_interrupted)
	{
		free(line);
		return (1);
	}
	if (ft_strcmp(line, delimiter) == 0)
	{
		free(line);
		return (1);
	}
	return (0);
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
	int		fd;

	fd = create_heredoc_file(env);
	if (fd == -1)
		return (-1);
	setup_heredoc_signals(env);
	env->heredoc_interrupted = 0;
	while (!env->heredoc_interrupted)
	{
		line = readline("heredoc> ");
		if (process_heredoc_line(line, delimiter, env))
			break ;
		write_line_to_heredoc(line, fd, env, expand_var);
	}
	return (cleanup_heredoc(fd, env));
}
