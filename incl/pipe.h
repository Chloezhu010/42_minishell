#ifndef PIPE_H
# define PIPE_H

# include "../incl/minishell.h"

typedef struct s_pipe
{
    pid_t pids[64];
    int pid_count;
    int prev_pipe_read;
    int pipefd[2];
    int stdin_backup;
    int stdout_backup;
} t_pipe;

void init_pipe(t_pipe *context);
void setup_pipe_input(t_cmd *cmd, t_pipe *ctx, t_env *env);
void setup_pipe_output(t_cmd *cmd, t_pipe *ctx, t_env *env);
void restore_pipe_io(t_pipe *ctx);
int create_pipe(t_pipe *ctx, t_env *env);
pid_t fork_process(t_env *env);
void execute_pipe_child(t_cmd *current, t_pipe *ctx, t_env *env);
void handle_pipe_parent(t_pipe *ctx, t_cmd *current, pid_t pid);
void wait_all_pipe(t_pipe *ctx, t_env *env);
void cleanup_pipe(t_pipe *ctx);
void execute_pipeline(t_cmd *cmds, t_env *env);

#endif