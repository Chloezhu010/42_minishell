/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auzou <auzou@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 13:10:36 by czhu              #+#    #+#             */
/*   Updated: 2025/03/12 15:07:32 by auzou            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/builtin.h"

/* init the built_in toggle
	- used in execute_shell
*/
t_builtin	*init_builtin(void)
{
	static t_builtin	builtin_in[]
	= {
				{"pwd", ft_pwd},
				{"cd", ft_cd},
				{"echo", ft_echo},
				{"env", ft_env},
				{"exit", ft_exit},
				{"export", ft_export},
				{"unset", ft_unset},
				{NULL, NULL}
				};
	return (builtin_in);
}

/* init the env from envp
    - count the env variables
    - malloc for env variables
    - copy env variables to my own struct
    - null terminated
*/
void	init_env(t_env *env, char **envp)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	if (!envp)
	{
		env->env_var = NULL;
		return ;
	}
	while (envp[count])
		count++;
	env->env_var = (char **)malloc((count + 1) * sizeof(char *));
	if (!env->env_var)
	{
		perror("malloc");
		exit (1);
	}
	while (envp[i])
	{
		env->env_var[i] = ft_strdup(envp[i]);
		i++;
	}
	env->env_var[i] = NULL;
	env->exit_status = 0;
}

/* printout the env variables */
void	print_env(t_env *env)
{
	int	i;

	i = 0;
	while (env && env->env_var && env->env_var[i])
	{
		printf("%s\n", env->env_var[i]);
		i++;
	}
}

/* free malloc for env */
void	free_env(t_env *env)
{
	int	i;

	i = 0;
	if (!env || !env->env_var)
		return ;
	while (env->env_var[i])
	{
		free(env->env_var[i]);
		i++;
	}
	free(env->env_var);
}

/* count the nbr of env_var */
int	count_env(t_env *env)
{
	int	i;

	i = 0;
	if (!env || !env->env_var)
		return (0);
	while (env->env_var[i])
		i++;
	return (i);
}
