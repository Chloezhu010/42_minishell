/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czhu <czhu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 16:23:05 by czhu              #+#    #+#             */
/*   Updated: 2025/03/29 16:50:31 by czhu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <signal.h>
# include <errno.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <sys/ioctl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <dirent.h>
# include <termios.h>
# include "../src/libft/libft.h"
# include "../incl/execution.h"
# include "../incl/builtin.h"
# include "../incl/loop.h"
# include "../incl/sig.h"

# define CHILD_PROCESS 0

typedef struct s_cmd	t_cmd;
typedef struct s_env	t_env;
typedef struct s_token	t_token;
typedef struct s_redir	t_redir;

/* pipe */
void	execute_pipeline(t_cmd *cmds, t_env *env);
void	ft_pipe(int pipefd[2]);
int		check_input_file(t_cmd *cmd, t_env *env);
int		create_output_file(t_cmd *cmd, t_env *env);

/* expander */
char	*extract_var_name(char *str);
char	*expand_var_instr(char *input, t_env *env);
void	expand_tokens(t_token *tokens, t_env *env);

/* expander utils */
char	*handle_regular_char(char *res, char current_char);
char	*handle_var_expansion(char *res, char **ptr, t_env *env);

/* redirect */
void	restore_io(int stdin_backup, int stdout_backup);
int		handle_heredoc(char *delimiter, t_env *env);
int		handle_input_redirect(t_cmd *cmd, int *stdin_backup, t_env *env);
int		handle_output_redirect(t_cmd *cmd, int *stdout_backup, t_env *env);
int		process_redirect(t_cmd *cmd, t_env *env);

/* execution */
void	execute_builtin(t_cmd *cmd, t_env *env);
void	execute_external(t_cmd *cmd, t_env *env);

/* shell loop */
char	*read_line(void);
char	**cell_split_line(char *line);
void	execute_cmd(t_cmd *cmd, t_env *env);
void	execute_shell(t_cmd *cmd, t_env *env);
void	shell_loop(t_env *env);

/* shell loop utils */
void	*ft_malloc(size_t size);
int		ft_strcmp(const char *s1, const char *s2);

/* execute_shell_utils*/
void	execute_external(t_cmd *cmd, t_env *env);
void	execute_cmd(t_cmd *cmd, t_env *env);
int		handle_redirect(t_cmd *cmd, int *stdin_backup,
			int *stdout_backup, t_env *env);
int		execute_builtin1(t_cmd *cmd, t_env *env,
			int stdin_backup, int stdout_backup);
void	execute_commands(t_cmd *cmds, t_env *env);
char	*get_command_line(void);
void	process_heredocs(t_cmd *cmds, t_env *env);

/* redirect_utils*/
int		handle_heredoc(char *delimiter, t_env *env);
int		handle_heredoc_redirect(t_cmd *cmd, int *stdin_backup, t_env *env);
int		handle_file_input_redirect(t_cmd *cmd, int *stdin_backup, t_env *env);
int		handle_input_redirect(t_cmd *cmd, int *stdin_backup, t_env *env);
int		open_output_file(t_cmd *cmd, t_env *env);
void	handle_redirect_error1(int fd, int *stdout_backup, t_env *env);
int		handle_output_redirect(t_cmd *cmd, int *stdout_backup, t_env *env);
void	restore_io(int stdin_backup, int stdout_backup);
int		handle_input_redirect1(t_redir *redir);
int		handle_output_redirect1(t_redir *redir);
void	merge_consecutive_tokens(t_token *tokens);
void	process_tokens_and_execute(t_token *tokens, t_env *env);
t_token	*process_command_line(char *line, t_env *env);

void	print_tokens(t_token *tokens);
void	print_cmds(t_cmd *cmds);
void	exit_status(t_env *env, int exit_status);
#endif
