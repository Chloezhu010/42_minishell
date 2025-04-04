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
	int	exit_status1;
	int	fd;
	int i;
	int	result;

	ft_putstr_fd("exit\n", 1);
	exit_status1 = 0;
	result = 0;
	i = 0;
	if (args[1] != NULL && args[2] != NULL)
	{
		ft_putstr_fd(" too many arguments\n", 2);
		if (args[1][0] == '-')
		{
			i = 1;
			while (args[1][i])
			{
				if (!ft_isdigit(args[1][i]))
				{
					exit_status(env, 2);
					break ;
				}
				i++;
			}
			exit_status(env, 1);
		}
		else
		{
			while (args[1][i])
			{
				if (!ft_isdigit(args[1][i]))
				{
					exit_status(env, 2);
					break ;
				}
				i++;
			}
			exit_status(env, 1);
		}
		return ;
	}
	if (args[1] != NULL)
	{	
		if (args[1][0] == '-')
		{
			i = 1;
			while (args[1][i])
			{
				result += (int)args[1][i++];
			}
		}
		else
		{
			i = 0;
			while (args[1][i])
			{
				result += (int)args[1][i++];
			}
		}
		// printf("%lld\n",ft_atoi(args[1]));
		if (is_digit(args[1]) == 0)
		{
			ft_putstr_fd(" numeric argument required\n", 2);
			exit_status(env, 2);
			env->exit_requested = 1; // set as 1 even on error
			return ;
		}
		else if (args[1][0] == '-')
		{
			if (result > 1001)
			{
				ft_putstr_fd("bash: exit: : numeric argument required\n", 2);
				exit_status1 = 2;
			}
			else
				exit_status1 = (int)ft_atoi(args[1]);
		}
		else
		{
			if (result > 1000)
			{
				ft_putstr_fd("bash: exit: : numeric argument required\n", 2);
				exit_status1 = 2;
			}
			else
				exit_status1 = (int)ft_atoi(args[1]);
		}
	}
	// clean up open fd
	fd = 3;
	while (fd < 10)
	{
		close(fd);
		fd++;
	}
	exit_status(env, exit_status1);
	env->exit_requested = 1;
}
