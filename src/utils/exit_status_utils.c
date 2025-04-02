/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_status_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auzou <auzou@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 19:34:10 by auzou             #+#    #+#             */
/*   Updated: 2025/03/31 19:39:26 by auzou            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

void	exit_status(t_env *env, int exit_status)
{
	if (!env->exit)
	{
		env->exit_status = exit_status;
		env->exit = 1;
	}
}
