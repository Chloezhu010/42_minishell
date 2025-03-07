#include "../../incl/minishell.h"

/* handle input redirect
    - handle heredoc
        - save original std input for restoration later
        - redirect stdin to the heredoc file
        - close fd_in to avoid resoruce leaks
    - hanlde other input redirect
        - (TODO)
*/
void handle_input_redirect(t_cmd *cmd, int *stdin_backup)
{
    if (cmd->heredoc && cmd->fd_in > 0)
    {
        *stdin_backup = dup(STDIN_FILENO);
        dup2(cmd->fd_in, STDIN_FILENO);
        close(cmd->fd_in);
    }
}

/* handle output redirect TODO */

/* restore std input after execution */
void restore_io(int stdin_backup)
{
    if (stdin_backup != -1)
    {
        dup2(stdin_backup, STDIN_FILENO);
        close(stdin_backup);
    }
}