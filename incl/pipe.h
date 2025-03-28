#ifndef PIPE_H
# define PIPE_H

# include "../incl/minishell.h"

/* struct for pipe context */
typedef struct s_pipe
{
    pid_t pids[64];
    int pid_count;
    int prev_pipe_read;
    int pipefd[2];
    int stdin_backup;
    int stdout_backup;
    int stderr_backup;
    int last_pid;
} t_pipe;

typedef struct s_child_pipe
{
    int stdin_backup;
    int stdout_backup;
    int redirect_error;
    int *pipefd;
} t_child_pipe;


void execute_pipeline(t_cmd *cmds, t_env *env);

void	init_pipe(t_pipe *context);
void	mark_pipeline_cmd(t_cmd *cmd);
int	create_cmd_pipe(t_pipe *ctx, t_env *env);
int	ft_dup(int fd, int *backup_ptr, char *name);
int ft_dup2(int oldfd, int newfd, int backup, char *name);
void	handle_redirect_error(t_cmd *cmd, t_pipe *ctx, int redirect_error, t_env *env);
void	handle_stderr_redirect(t_cmd *cmd);
int	setup_pipe_input(t_cmd *cmd, t_pipe *ctx, int *stdin_backup, t_env *env);
int	setup_pipe_output(t_cmd *cmd, t_pipe *ctx, int *stdout_backup, t_env *env);
int	setup_pipe_output(t_cmd *cmd, t_pipe *ctx, int *stdout_backup, t_env *env);
void	execute_pipe_child(t_cmd *current,
		t_pipe *ctx, t_env *env, int redirect_error);
void	execute_parent_process(t_pipe *ctx, t_cmd *cmd, pid_t pid);
void	restore_std_fd(t_pipe *ctx);
void	wait_for_child(t_pipe *ctx, t_env *env);

void debug_print_cmd(t_cmd *cmd);
#endif