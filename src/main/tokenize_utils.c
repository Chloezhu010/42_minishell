/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_utile.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auzou <auzou@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 21:01:36 by auzou             #+#    #+#             */
/*   Updated: 2025/02/27 21:08:33 by auzou            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/loop.h"

int	ft_isspace(char a)
{
	if (a == ' ' || a == '\t' || a == '\n')
		return (1);
	return (0);
}

int	is_special_char(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == '&');
}
