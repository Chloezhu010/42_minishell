/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czhu <czhu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 17:57:10 by auzou             #+#    #+#             */
/*   Updated: 2025/03/25 12:02:48 by czhu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

/* helper function to handle regular char in expand_var_instr
    - create a temp str with current_char + \0
    - append the char to the res string
    - free the old res string
    - return the new temp string
*/
char	*handle_regular_char(char *res, char current_char)
{
	char	str[2];
	char	*temp;

	str[0] = current_char;
	str[1] = '\0';
	temp = ft_strjoin(res, str);
	free(res);
	return (temp);
}

/*	expand a single env_var
	- use case
		- expand $USER to user_name in env
	- implementation
	- if str is null, empty or space
		- return ""
	- if no $, copy the str as it is
	- if contains $
		- if "$?", return the exit value
		- if "$" + other cases
			- skip the $ char
			- search the parameter in env_var
*/

static char	*expand_variable(const char *str, t_env *env)
{
	int		i;
	int		key_len;
	char	*varname;

	if (!str || str[0] == '\0')
		return (ft_strdup(""));
	if (str[0] == '$' && str[1] == '\0')
		return (ft_strdup("$"));
	if (str[0] != '$')
		return (ft_strdup(str));
	if (ft_strcmp(str, "$?") == 0)
		return (ft_itoa(env->exit_status));
	varname = (char *)str + 1;
	key_len = ft_strlen(varname);
	i = 0;
	while (env->env_var && env->env_var[i])
	{
		if (ft_strncmp(env->env_var[i], varname, key_len) == 0
			&& env->env_var[i][key_len] == '=')
			return (ft_strdup(env->env_var[i] + key_len + 1));
		i++;
	}
	return (ft_strdup(""));
}

/* helper function to handle var expansion in expand_var_instr
    - extract the var name
    - expand the var name
    - if the varis expanded successfully
        - append the expanded value to the res str
        - free the old res str and update with new one
        - free the expanded value
    - move the ptr pass the var name
    - free the extracted var name
*/
char	*handle_var_expansion(char *res, char **ptr, t_env *env)
{
	char	*var_name;
	char	*expanded;
	char	*temp;

	var_name = extract_var_name(*ptr);
	expanded = expand_variable(var_name, env);
	if (expanded)
	{
		temp = ft_strjoin(res, expanded);
		free(res);
		res = temp;
		free(expanded);
	}
	*ptr += ft_strlen(var_name);
	free(var_name);
	return (res);
}

void	expand_tokens(t_token *tokens, t_env *env)
{
	t_token	*current;
	char	*expanded;

	current = tokens;
	while (current)
	{
		if (current->type == TOKEN_DOUBLE_QUOTE || current->type == TOKEN_WORD)
		{
			expanded = expand_var_instr(current->value, env);
			if (expanded)
			{
				free(current->value);
				current->value = expanded;
			}
		}
		current = current->next;
	}
	current = tokens;
	while (current)
	{
		current = current->next;
	}
}
