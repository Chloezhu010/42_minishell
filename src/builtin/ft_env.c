/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czhu <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 13:20:22 by czhu              #+#    #+#             */
/*   Updated: 2025/02/18 16:34:28 by czhu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/builtin.h"

/* env with no option
    env request
    - print out the env info
    env implementation
    - wrap the init, print, free functions
*/
void	ft_env(char **args, t_env *env)
{
	(void)args;
	if (!env || !env->env_var)
	{
		printf("env not initialized\n");
		return ;
	}
	print_env(env);
}
/*
// === test env ===
int main(int ac, char **av, char **envp)
{
    t_env env;
    
    (void)ac;
    (void)av;
    ft_env(&env, envp);
    return (0);
}*/
