/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chloe <chloe@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 17:41:16 by czhu              #+#    #+#             */
/*   Updated: 2025/03/25 23:17:23 by chloe            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/builtin.h"

/*
    cd with only a relative or absolute path
    - implementation
        - init env
        - input control
            - if ac = 1, dir = HOME
            - if ac = 2 or more than 2, dir = args[1]
        - store current dir for OLDPWD
        - change dir using chdir
            - if succeed, chdir return 0
            - if fail, return -1
        - update the OLDPWD and PWD env var
*/
static char	*resolve_directory(char **args, t_env *env)
{
	char	*dir;

	(void)env;
	if (!args[1])
	{
		dir = getenv("HOME");
		if (!dir)
		{
			printf("cd: HOME not set\n");
			return (NULL);
		}
	}
	else
	{
		dir = args[1];
	}
	return (dir);
}

static void	execute_cd(char *dir, t_env *env)
{
	char	cwd[1024];

	if (!dir)
		return ;
	if (getcwd(cwd, sizeof(cwd)) == NULL)
		perror("getcwd");
	if (chdir(dir) != 0)
	{
		env->exit_status = 1;
		perror("cd");
	}
	else
	{
		update_env("OLDPWD", cwd, env);
		if (getcwd(cwd, sizeof(cwd)) != NULL)
			update_env("PWD", cwd, env);
	}
}

void	ft_cd(char **args, t_env *env)
{
	char	*dir;

	if (!env)
		return ;
	dir = resolve_directory(args, env);
	execute_cd(dir, env);
}
/*
// // === test create_env_entry ===
// int main()
// {
//     char *key = "PWD";
//     char *value = "/home/user";
//     char *new_entry = create_env_entry(key, value);
//     printf("%s\n", new_entry);
// }

// === test cd ===
int main(int ac, char **av, char **envp)
{    
    t_env env;
    env.env_var = NULL;
    
    (void)ac;
    ft_cd(av, &env, envp);

    // char cwd[1024];
    // if (getcwd(cwd, sizeof(cwd)) != NULL)
    //     printf("current dir: %s\n\n", cwd);
    // else
    //     perror("cd");
    // === check env status ===
    printf("=== print out env to check ===\n");
    print_env(&env);

    // free up memory
    free_env(&env);
    return (0);
}*/
