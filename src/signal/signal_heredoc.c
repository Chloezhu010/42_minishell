/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czhu <czhu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 12:20:33 by czhu              #+#    #+#             */
/*   Updated: 2025/04/01 14:10:43 by czhu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/sig.h"

// void    setup_heredoc_signals(t_env *env)
// {
//     struct sigaction    sa;

//     set_signal_env(env);
//     disable_echo();
//     sigemptyset(&sa.sa_mask);
//     sa.sa_handler = heredoc_sigint_handler;
//     sa.sa_flags = 0;
//     sigaction(SIGINT, &sa, NULL);

//     sa.sa_handler = SIG_IGN;
//     sigaction(SIGQUIT, &sa, NULL);
// }