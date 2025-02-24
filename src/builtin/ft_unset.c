/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czhu <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 11:16:25 by czhu              #+#    #+#             */
/*   Updated: 2025/02/18 16:30:37 by czhu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/builtin.h"

/* cal the length of the environment array */
int	ft_env_len(t_env *env)
{
	int	len;

	len = 0;
	if (!env || !env->env_var)
		return (0);
	while (env->env_var[len])
		len++;
	return (len);
}

/* remove the var from the array
	- loop through the env array
	- strncmp the input name and the env_var array
	- if match
		- free the env_var[i] memory
		- shift the array forward
	- if not
		- do nothing
*/
void	ft_unsetenv(char *name, t_env *env)
{
	int	len;
	int	i;
	int	j;

	len = ft_env_len(env);
	i = 0;
	while (env->env_var[i])
	{
		if (strncmp(env->env_var[i], name, ft_strlen(name)) == 0
			&& env->env_var[i][ft_strlen(name)] == '=')
		{
			free(env->env_var[i]);
			j = i;
			while (j < len - 1)
			{
				env->env_var[j] = env->env_var[j + 1];
				j++;
			}
			env->env_var[len - 1] = NULL;
			return ;
		}
		i++;
	}
}

/* check if the variable name is valid
	- only accept aphabetic & numeric arg or "_"
*/
int	is_valid_name(char *name)
{
	int	i;

	if (!name || !*name)
		return (0);
	i = 1;
	while (name[i])
	{
		if (ft_isalnum(name[i]) != 1 && name[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

/* unset with no option
	- check argument
		- need to have >= 1 arg
		- only accept alphabetic & numeric input
	- if the env struct has not been initialized
		- init & store the envp from the main
	- loop through the arg array
		- if arg[i] invalid
			- print out error msg
			- continue to the next arg
		- if arg valid
			- call ft_unsetenv to remove the var
*/
void	ft_unset(char **av, t_env *env, char **envp)
{
	int	i;

	if (!av || !env)
		return ;
	if (!av[1])
	{
		printf("unset: not enough arguments\n");
		return ;
	}
	if (!env->env_var)
		init_env(env, envp);
	i = 1;
	while (av[i])
	{
		if (!is_valid_name(av[i]))
		{
			printf("unset: %s: not a valid identifier\n", av[i]);
			i++;
			continue ;
		}
		ft_unsetenv(av[i], env);
		i++;
	}
}
/*
// == test is_valid_name ==
int	main(int ac, char **av)
{
	(void)ac;

	printf("%d\n", is_valid_name(av[1]));
	return (0);
}

// === test ft_unset ===
int main(int ac, char **av, char **envp)
{
    t_env env;

	(void)ac;
	ft_unset(av, &env, envp);

	// printout env after ft_unset to check
	int i = 0;
	while (env.env_var && env.env_var[i])
	{
		printf("%s\n", env.env_var[i]);
		i++;
	}

	// freeup in the end
	free_env(&env);
	return (0);
}*/
