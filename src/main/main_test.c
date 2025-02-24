#include "../incl/minishell.h"

void    shell_loop(void)
{
    char    *input;
    t_command   *cmds; // parsed command structure

    while (1)
    {
        // 1. display the prompt and read user input
        input = readline("minishell$> ");
        if (!input)
        {
            ft_printf("exit\n"); // handle ctl+D to exit
            break ;
        }
        // 2. add non-empty input to history
        if (*input != '\0')
            add_history(input);
        // 3. parse input in a command structure
        cmds = parse_input(input); // TODO
        free(input); // free the raw input after parsing
        if (!cmds)
            continue ; 
        // 4. execute parsed commands
        execute_commands(cmds); // TODO
        // 5. cleanup parsed command structure
        free_commands(cmds); // TODO
    }
    return (0);
}

int main(int ac, char **av, char **envp)
{
    (void)ac;
    (void)av;

    // 1. init env and signal handling
    init_shell(envp);
    setup_signals();
    // 2. enter the shell loop
    shell_loop();
    // 3. cleanup before exit
    cleanup_shell();
    // 4. loop until exit
    exit();
    return (0);
}
