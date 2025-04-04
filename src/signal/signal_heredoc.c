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

void    setup_heredoc_signals(t_env *env)
{
    struct sigaction    sa_int;
    struct sigaction    sa_quit;

    set_signal_env(env);
    disable_echo();

    // handle ctl + C
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_handler = heredoc_sigint_handler;
    sa_int.sa_flags = 0;
    sigaction(SIGINT, &sa_int, NULL);

    // ignore ctl + backslash
    sigemptyset(&sa_quit.sa_mask);
    sa_quit.sa_handler = SIG_IGN;
    sa_quit.sa_flags = 0;
    sigaction(SIGQUIT, &sa_quit, NULL);
}