/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auzou <auzou@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 16:56:07 by czhu              #+#    #+#             */
/*   Updated: 2025/03/25 17:10:29 by auzou            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/builtin.h"

/* check if a string is a valid numeric arg */
int	is_digit(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

/* exit with no option
	requirement
	- need to exit with numeric arg or no arg
	- take only 1 arg
		- 0: success
		- non zero: error
*/
void	ft_exit(char **args, t_env *env)
{
	int	exit_status;

	ft_putstr_fd("exit\n", 1);
	exit_status = 0;
	if (args[1] != NULL && args[2] != NULL)
	{
		ft_putstr_fd(" too many arguments\n", 2);
		env->exit_status = 1;
		return ;
	}
	if (args[1] != NULL)
	{
		if (is_digit(args[1]) == 0)
		{
			ft_putstr_fd(" numeric argument required\n", 2);
			env->exit_status = 2;
			return ;
		}
		else
		{
			exit_status = ft_atoi(args[1]);
		}
	}
	env->exit_status = exit_status;
	env->exit_requested = 1;
}
/*
// === testing exit ===
int main(int ac, char **av)
{
	(void)ac;
	ft_exit(av);
}*/
