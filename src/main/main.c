/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auzou <auzou@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 17:58:13 by auzou             #+#    #+#             */
/*   Updated: 2025/03/25 17:10:43 by auzou            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

int	main(int ac, char **av, char **envp)
{
	t_env	env;

	(void)ac;
	(void)av;
	env.exit_requested = 0;
	env.exit = 0;
	init_env(&env, envp);
	add_shlvl(&env);
	shell_loop(&env);
	free_env(&env);
	enable_echo();
	rl_clear_history();
	return (env.exit_status);
}

void	shell_loop(t_env *env)
{
	char	*line;
	t_token	*tokens;

	setup_signal(env);
	while (1)
	{
		env->exit = 0;
		env->at_prompt = 1;
		line = get_command_line();
		env->at_prompt = 0;
		if (line == NULL)
			break ;
		tokens = process_command_line(line, env);
		print_tokens(tokens);
		if (tokens)
			process_tokens_and_execute(tokens, env);
		free(line);
		if (env->exit_requested)
			break ;
		if (!env->exit)
			env->exit_status = 0;
	}
}
