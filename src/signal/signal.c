/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auzou <auzou@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 13:16:34 by czhu              #+#    #+#             */
/*   Updated: 2025/03/21 18:53:04 by auzou            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/sig.h"
/* setup signals
    - disable echo
    - handle SIGINT
    - handle SIGQUIT
*/

void	setup_signal(t_env *env)
{
	struct sigaction	sa;

	set_signal_env(env);
	disable_echo();
	sigemptyset(&sa.sa_mask);
	sa.sa_handler = sigint_handler;
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sigemptyset(&sa.sa_mask);
	sa.sa_handler = sigquit_handler;
	sa.sa_flags = 0;
	sigaction(SIGQUIT, &sa, NULL);
}
