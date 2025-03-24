// /* helper function: check if input files exist for a cmd
//     - return 1 for error
//     - return 0 for success
// */
// int	check_input_file(t_cmd *cmd, t_env *env)
// {
// 	t_redir	*redir;
// 	int		fd;

// 	redir = cmd->redirects;
// 	while (redir)
// 	{
// 		if (redir->type == TOKEN_REDIRECT_IN)
// 		{
// 			fd = open(redir->file, O_RDONLY);
// 			if (fd == -1)
// 			{
// 				perror("minishell");
// 				env->exit_status = 1;
// 				return (1);
// 			}
// 			close(fd);
// 		}
// 		redir = redir->next;
// 	}
// 	return (0);
// }

/* helper function: create output files for a cmd */
// int	create_output_file(t_cmd *cmd, t_env *env)
// {
// 	t_redir	*redir;
// 	int		fd;

// 	redir = cmd->redirects;
// 	while (redir)
// 	{
// 		if (redir->type == TOKEN_REDIRECT_OUT
// 			|| redir->type == TOKEN_REDIRECT_APPEND)
// 		{
// 			if (redir->type == TOKEN_REDIRECT_APPEND)
// 				fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
// 			else
// 				fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 			if (fd == -1)
// 			{
// 				perror("minishell");
// 				env->exit_status = 1;
// 				return (1);
// 			}
// 			close(fd);
// 		}
// 		redir = redir->next;
// 	}
// 	return (0);
// }

// /*
//     - create a temp file to store input line
//         - write only
//         - if not exits, create one
//         - if the file exists, content will be erased
//     - read user input until the delimiter
//         - input lines are read & written to the temp file
//         - close and reopen with read-only mode
//     - reopen the file and return fd
// */
// int	handle_heredoc(char *delimiter, t_env *env)
// {
// 	char	*line;
// 	char	*expanded_line;
// 	int		fd;

// 	fd = open("/tmp/minishell_heredoc", O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 	if (fd == -1)
// 	{
// 		perror("open");
// 		env->exit_status = 1;
// 		return (-1);
// 	}
// 	while (1)
// 	{
// 		line = readline("heredoc> ");
// 		if (!line || ft_strcmp(line, delimiter) == 0)
// 			break ;
// 		expanded_line = expand_var_instr(line, env);
// 		write(fd, expanded_line, ft_strlen(expanded_line));
// 		write(fd, "\n", 1);
// 		free(expanded_line);
// 		free(line);
// 	}
// 	close(fd);
// 	fd = open("/tmp/minishell_heredoc", O_RDONLY);
// 	return (fd);
// }