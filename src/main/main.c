/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czhu <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 11:28:56 by czhu              #+#    #+#             */
/*   Updated: 2025/02/16 11:33:27 by czhu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

/* main function
    - arg control
    - init the minishell (setup env, signal etc.)
    - REPL loop
        - parse input into command structures (args, redirections, pipes etc.)
        - execute commands
            - handle builtin (eg. cd, exit)
            - execute external programs via execve
            - handle redirections and pipes
    - cleanup memory after each command
    - loop until exit
*/
int	main(int ac, char **av, char **envp)
{
	
    (void)envp;
	if (ac != 1 || av[1])
	{
		ft_printf("This program doesn't take any argument\n");
		exit (1);
	}
	// init_shell();
	// shell_loop();
	// cleanup();
	// exit();
	return (0);
}
