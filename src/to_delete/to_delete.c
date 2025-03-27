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