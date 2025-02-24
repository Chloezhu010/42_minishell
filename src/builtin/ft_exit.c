/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czhu <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 16:56:07 by czhu              #+#    #+#             */
/*   Updated: 2025/02/16 16:56:28 by czhu             ###   ########.fr       */
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
void ft_exit(char **args, t_env *env, char **envp)
{
	int	exit_status;

	(void)env;
	(void)envp;
	exit_status = 0;
	if (args[1] != NULL && args[2] != NULL)
		printf("exit: too many arguments\n");
	if (args[1] != NULL)
	{
		if (is_digit(args[1]) == 0)
		{
			printf("exit: numeric argument is needed\n");
			exit_status = 2;
		}
		exit_status = ft_atoi(args[1]);
	}
	exit(exit_status);
}
// === testing exit ===
// int main(int ac, char **av)
// {
// 	(void)ac;
// 	ft_exit(av);
// }