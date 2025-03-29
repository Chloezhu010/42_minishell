/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auzou <auzou@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 17:58:16 by auzou             #+#    #+#             */
/*   Updated: 2025/03/26 23:25:31 by auzou            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

char	*read_line(void)
{
	char	*buf;

	buf = NULL;
	buf = readline("minishell $>");
	return (buf);
}

char	**cell_split_line(char *line)
{
	char	**tokens;

	if (!line)
		return (NULL);
	tokens = ft_split(line, ' ');
	return (tokens);
}

void	execute_shell(t_cmd *cmd, t_env *env)
{
	int		stdin_backup;
	int		stdout_backup;
	int		status;
	pid_t	pid;

	stdin_backup = -1;
	stdout_backup = -1;
	if (!cmd->args[0] || !env)
		return ;
	if (handle_redirect(cmd, &stdin_backup, &stdout_backup, env))
		return ;
	if (execute_builtin1(cmd, env, stdin_backup, stdout_backup))
		return ;
	pid = ft_fork();
	if (pid == CHILD_PROCESS)
		execute_cmd(cmd, env);
	else
	{
		ft_wait(&status);
		if (WIFEXITED(status))
			env->exit_status = WEXITSTATUS(status);
	}
	restore_io(stdin_backup, stdout_backup);
}

t_token *process_command_line(char *line, t_env *env)
{
    t_token *tokens;

    if (!line || !*line)
        return (NULL);
    add_history(line);
    tokens = tokenize(line);
    if (!tokens)
        return (NULL);
    expand_tokens(tokens, env);
    merge_consecutive_tokens(tokens); // 新添加的函数调用
    check_format_command(tokens);
    return (tokens);
}

/* shell loop
    - setup signal handler
    - read line from the command
        - add non-empty line to history
    - parse the args
    - execute the args
    - cleanup before exit
*/


/* 打印命令链表的内容 */
// void    print_cmds(t_cmd *cmd)
// {
//     int     i;
//     t_cmd   *current;
//     t_redir *redir;

//     current = cmd;
//     printf("\n===== COMMANDS =====\n");
//     while (current)
//     {
//         printf("Command: ");
//         i = 0;
//         if (current->args)
//         {
//             while (current->args[i])
//             {
//                 printf("[%s] ", current->args[i]);
//                 i++;
//             }
//         }
//         else
//             printf("(no args)");
//         printf("\n");

//         if (current->infile)
//             printf("Infile: %s\n", current->infile);
//         if (current->outfile)
//             printf("Outfile: %s (append: %d)\n", current->outfile, current->append);
//         if (current->heredoc)
//             printf("Heredoc with delimiter: %s (fd: %d)\n", current->delimiter, current->fd_in);
        
//         printf("In pipe: %d\n", current->in_pipe);
        
//         /* 打印所有重定向 */
//         if (current->redirects)
//         {
//             printf("Redirections:\n");
//             redir = current->redirects;
//             while (redir)
//             {
//                 switch (redir->type)
//                 {
//                     case TOKEN_REDIRECT_IN: 
//                         printf("  < %s\n", redir->file);
//                         break;
//                     case TOKEN_REDIRECT_OUT: 
//                         printf("  > %s\n", redir->file);
//                         break;
//                     case TOKEN_REDIRECT_APPEND: 
//                         printf("  >> %s\n", redir->file);
//                         break;
//                     case TOKEN_HEREDOC: 
//                         printf("  << %s\n", redir->file);
//                         break;
//                 }
//                 redir = redir->next;
//             }
//         }
        
//         printf("-------------------\n");
//         current = current->next;
//     }
//     printf("====================\n\n");
// }

void	shell_loop(t_env *env)
{
	char	*line;
	t_token	*tokens;
	t_cmd	*cmds;

	setup_signal(env);
	while (1)
	{
		env->at_prompt = 1; //add
		line = get_command_line();
		env->at_prompt = 0; //add
		if (line == NULL)
			break ;
		tokens = process_command_line(line, env);
		if (tokens)
		{
			cmds = parse_tokens(tokens);
			free_tokens(tokens);
			if (cmds)
			{
				process_heredocs(cmds, env);
				execute_commands(cmds, env);
				// print_cmds(cmds);
				free_cmds(cmds);
			}
			// print_tokens(tokens);
			
		}
		free(line);
		if (env->exit_requested)
			break ;
	}
}
