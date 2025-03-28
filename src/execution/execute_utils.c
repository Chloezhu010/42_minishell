/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czhu <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 13:19:05 by czhu              #+#    #+#             */
/*   Updated: 2025/03/02 13:19:39 by czhu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/execution.h"

/* fork function wrapper */
pid_t	ft_fork(void)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		exit(1);
	}
	return (pid);
}

/* execve function wrapper
    - check input: path, av
    - check env init
    - use execve to execute the cmd
        - if execve fails (returns -1), print error and exit
*/
static int	check_execve_errors(char *path, t_env * env)
{
	DIR			*dir;
	struct stat	path_stat;

	dir = opendir(path);
	if (dir != NULL)
	{
		closedir(dir);
		write(2, "execve failed: Is a directory\n", 31);
		env->exit_status = 126;
		return 0;
	}
	if (stat(path, &path_stat) == -1)
	{
		write(2, "execve failed: No such file or directory\n", 42);
		env->exit_status = 127;
		return 0;
	}
	if (access(path, X_OK) == -1)
	{
		write(2, "execve failed: Permission denied\n", 34);
		env->exit_status = 126;
		return 0;
	}
	ft_putstr_fd(" command not found\n", 2);
	env->exit_status = 127;
	return (0);
}

void	ft_execve(char *path, char **av, t_env *env)
{
	if (!path || !av || !av[0])
	{
		write(2, "execve: invalid args\n", 21);
		env->exit_status = 1;
		return ;
	}
	if (!env || !env->env_var)
	{
		write(2, "env not initialized\n", 20);
		env->exit_status = 1;
		return ;
	}
	if (execve(path, av, env->env_var) == -1)
		check_execve_errors(path, env);
}

/* wait function wrapper
    - check status
    - if wait fails, print error and return result
*/
pid_t	ft_wait(int *status)
{
	pid_t	result;

	if (!status)
	{
		printf("wait: status arg required\n");
		return (-1);
	}
	while (1)
    {
        result = wait(status);
        if (result == -1)
        {
            if (errno == EINTR)
                continue;  // If interrupted by a signal, try again
            else
                perror("wait failed");  // Only report other errors
        }
        break;
    }
    
    // return (result);
	// result = wait(status);
	// if (result == -1)
	// 	perror("wait failed");
	return (result);
}
