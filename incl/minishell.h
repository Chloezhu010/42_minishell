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

typedef struct s_cmd t_cmd;
typedef struct s_env t_env;
typedef struct s_token t_token;

/* pipe */
void execute_pipeline(t_cmd *cmds, t_env *env);
void ft_pipe(int pipefd[2]);
int check_input_file(t_cmd *cmd, t_env *env);
int create_output_file(t_cmd *cmd, t_env *env);

/* expander */
char *extract_var_name(char *str);
char *expand_var_instr(char *input, t_env *env);
void	expand_tokens(t_token *tokens, t_env *env);

/* expander utils */
char *handle_regular_char(char *res, char current_char);
char *handle_var_expansion(char *res, char **ptr, t_env *env);

/* redirect */
void restore_io(int stdin_backup, int stdout_backup);
int handle_heredoc(char *delimiter, t_env *env);
int handle_input_redirect(t_cmd *cmd, int *stdin_backup, t_env *env);
int handle_output_redirect(t_cmd *cmd, int *stdout_backup, t_env *env);
int process_redirect(t_cmd *cmd, t_env *env);

/* shell loop */
char *read_line(void);
char    **cell_split_line(char *line);
void launch_execution(char **args, t_env *env);
void execute_shell(t_cmd *cmd, t_env *env);
void shell_loop(t_env *env);

/* shell loop utils */
void	*ft_malloc(size_t size);
int		ft_strcmp(const char *s1, const char *s2);

#endif
