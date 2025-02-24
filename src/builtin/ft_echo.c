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

/* echo with -n option: print argus
	- start from args[1], skip "echo"
	- set new_line as default 1
    - handle -n option
		- if yes, set new_line to 0
	- normal case: print out the args
    - print out the newline in the end
*/
void	ft_echo(char **args, t_env *env, char **envp)
{
	int	i;
	int	new_line;

	(void)env;
	(void)envp;
	i = 1;
	new_line = 1;
	while (args[i] && args[i][0] == '-')
	{
		if (ft_strncmp(args[i], "-n", ft_strlen(args[i])) == 0)
			new_line = 0;
		else
			break ;
		i++;
	}
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	if (new_line == 1)
		printf("\n");
}
/*
// === testing echo ===
int main(int ac, char **av)
{
    (void)ac;
    ft_echo(av);
}*/
