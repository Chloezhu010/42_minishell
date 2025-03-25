/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czhu <czhu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 13:10:36 by czhu              #+#    #+#             */
/*   Updated: 2025/03/25 14:31:33 by czhu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/builtin.h"

/* init the built_in toggle
	- used in execute_shell
*/
t_builtin	*init_builtin(void)
{
	static t_builtin	builtin_in[] = {
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

/* count the nbr of env var */
int count_envp(char **envp)
{
	int count;

	count = 0;
	if (!envp)
		return (0);
	while (envp[count])
		count++;
	return (count);
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

	if (!envp)
	{
		env->env_var = NULL;
		env->exported = NULL;
		env->exit_status = 0;
		env->at_prompt = 0;
		return ;
	}
	i = 0;
	count = count_envp(envp);
	env->env_var = (char **)ft_malloc((count + 1) * sizeof(char *));
	// if (!env->env_var)
	// {
	// 	perror("malloc");
	// 	exit (1);
	// }
	while (envp[i])
	{
		env->env_var[i] = ft_strdup(envp[i]);
		i++;
	}
	env->env_var[i] = NULL;
	env->exported = NULL;
	env->exit_status = 0;
	env->at_prompt = 0; //add
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
	if (env->exported)
		free(env->exported);
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
