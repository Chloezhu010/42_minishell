/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auzou <auzou@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 18:21:07 by auzou             #+#    #+#             */
/*   Updated: 2025/03/21 18:38:52 by auzou            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

int	handle_output_redirect_result(t_cmd *cmd, t_env *env, int result)
{
	if (result == 1)
	{
		if (!cmd->in_pipe)
			env->exit_status = 1;
		return (1);
	}
	return (0);
}

int	process_redirect(t_cmd *cmd, t_env *env)
{
	t_redir	*redir;
	int		result;

	redir = cmd->redirects;
	while (redir)
	{
		if (redir->type == TOKEN_REDIRECT_IN)
		{
			result = handle_input_redirect1(redir);
			if (handle_output_redirect_result(cmd, env, result) == 1)
				return (1);
		}
		else if (redir->type == TOKEN_REDIRECT_APPEND
			|| redir->type == TOKEN_REDIRECT_OUT)
		{
			result = handle_output_redirect1(redir);
			if (handle_output_redirect_result(cmd, env, result) == 1)
				return (1);
		}
		redir = redir->next;
	}
	return (0);
}

// // === test redirect ===
// int main()
// {
//     t_cmd cmd;
//     int stdin_backup = -1;
//     int stdout_backup = -1;

//     // Initialize the command structure
//     cmd.infile = NULL;
//     cmd.outfile = NULL;
//     cmd.append = 0;
//     cmd.heredoc = 0;
//     cmd.delimiter = NULL;
//     cmd.fd_in = -1;

//     // === test input redirect (cat < input.txt)
//     printf("test input redirection\n");
//     cmd.infile = "input.txt";
//     if (handle_input_redirect(&cmd, &stdin_backup) == -1)
//     {
//         fprintf(stderr, "input redirect failed\n");
//         return (1);
//     }
//     // simulate a cmd that reads from stdin
//     char buffer[1024];
//     ssize_t bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer)-1);
//     if (bytes_read > 0)
//     {
//         buffer[bytes_read] = '\0';
//         printf("read from input file: %s\n", buffer);
//     }
//     else
//         perror("read");
//     restore_io(stdin_backup, stdout_backup);
//     // === test output rediret
//     printf("\ntest output redirection\n");
//     cmd.outfile = "output.txt";
//     cmd.append = 0;
//     if (handle_output_redirect(&cmd, &stdout_backup) == -1)
//     {
//         fprintf(stderr, "output redirect failed\n");
//         return (1);
//     }
//     // simulate a cmd that writes to stdout
//     printf("hello \n");
//     restore_io(stdin_backup, stdout_backup);

//     // === test append mode
//     printf("\ntest output redirection - append mode\n");
//     cmd.outfile = "output.txt";
//     cmd.append = 1;
//     if (handle_output_redirect(&cmd, &stdout_backup) == -1)
//     {
//         fprintf(stderr, "output redirect failed\n");
//         return (1);
//     }
//     printf("school42\n");
//     restore_io(stdin_backup, stdout_backup);
//     // === test heredoc
//     printf("\ntest heredoc\n");
//     cmd.heredoc = 1;
//     cmd.delimiter = "eof";
//     cmd.fd_in = handle_heredoc(cmd.delimiter);
//     if (cmd.fd_in == -1)
//     {
//         fprintf(stderr, "heredoc failed\n");
//         return (1);
//     }
//     // simuate a cmd that reads from stdin
//     bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer)-1);
//     if (bytes_read > 0)
//     {
//         buffer[bytes_read] = '\0';
//         printf("read from input file: %s\n", buffer);
//     }
//     else
//         perror("read");
//     restore_io(stdin_backup, stdout_backup);
// }