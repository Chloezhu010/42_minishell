/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sig.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czhu <czhu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 16:23:13 by czhu              #+#    #+#             */
/*   Updated: 2025/03/29 16:48:20 by czhu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIG_H
# define SIG_H
# include "../incl/minishell.h"

void	set_signal_env(t_env *env);
void	disable_echo(void);
void	enable_echo(void);
void	sigint_handler(int sig);
void	sigquit_handler(int sig);
void	setup_signal(t_env *env);
void	reset_input_state(void);

#endif
