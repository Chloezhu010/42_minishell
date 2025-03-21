/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auzou <auzou@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 17:58:13 by auzou             #+#    #+#             */
/*   Updated: 2025/03/21 17:58:20 by auzou            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

int	main(int ac, char **av, char **envp)
{
	t_env	env;

	(void)ac;
	(void)av;
	init_env(&env, envp);
	shell_loop(&env);
	free_env(&env);
	enable_echo();
	rl_clear_history();
	return (env.exit_status);
}
