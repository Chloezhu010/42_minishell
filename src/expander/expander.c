/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auzou <auzou@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 17:57:23 by auzou             #+#    #+#             */
/*   Updated: 2025/03/21 17:57:56 by auzou            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

/* extract the var name from a double quoted str
	- use case
		- extract $USER from "hi $USER this is 42"
	- implementation
		- find the $ sign using ft_strchr
		- find the var_end
			- whenever it's alpha numberic or "_", move the ptr forward
*/
char	*extract_var_name(char *str)
{
	char	*var_start;
	char	*var_end;

	var_start = ft_strchr(str, '$');
	if (!var_start)
		return (NULL);
	var_end = var_start + 1;
	while (*var_end && !ft_isspace(*var_end))
	{
		if (*var_end && (*var_end == '?'
				|| (*var_end >= '0' && *var_end <= '9')))
		{
			var_end++;
			break ;
		}
		if ((*var_end >= 'A' && *var_end <= 'Z') || *var_end == '_')
		{
			while ((*var_end >= 'A' && *var_end <= 'Z')
				|| *var_end == '_' || (*var_end >= '0' && *var_end <= '9'))
				var_end++;
			break ;
		}
	}
	return (strndup(var_start, var_end - var_start));
}

/* expand var in a str
	- set up an empty str res
	- loop through the input str
		- if encounter $
			- call handle_var_expansion
		- if not (a regular string)
			- call handle_regular_char
*/
char	*expand_var_instr(char *input, t_env *env)
{
	char	*res;
	char	*ptr;

	ptr = input;
	res = ft_strdup("");
	while (*ptr)
	{
		if (*ptr == '$')
			res = handle_var_expansion(res, &ptr, env);
		else
		{
			res = handle_regular_char(res, *ptr);
			ptr++;
		}
	}
	return (res);
}

/*  expand var in the token array
	- skip single quoted string
	- handle double quoted string or normal string
		- expand var using expand_var_instr
		- if expand successfully
			- free current token value
			- update with the expanded value
		- move to the next token
*/
void merge_consecutive_tokens(t_token *tokens)
{
    t_token *current;
    t_token *next;
    char *combined;

    if (!tokens)
        return;

    current = tokens;
    while (current && current->next)
    {
        // 如果当前token标记为连续
        if (current->consecutive_quote)
        {
            next = current->next;
            
            // 调试输出
            printf("合并tokens: [%s] 和 [%s]\n", current->value, next->value);
            
            // 合并两个token的内容
            combined = ft_strjoin(current->value, next->value);
            if (combined)
            {
                // 更新当前token的值
                free(current->value);
                current->value = combined;
                
                // 继承下一个token的连续标记，确保可以处理多个连续token
                current->consecutive_quote = next->consecutive_quote;
                
                // 从链表中移除下一个token
                current->next = next->next;
                free(next->value);
                free(next);
                
                // 不前进到下一个token，保持在当前位置
                // 这样可以处理多个连续token（例如：123"abc"'def'"ghi"）
                continue;
            }
        }
        
        current = current->next;
    }
}

void expand_tokens(t_token *tokens, t_env *env)
{
    t_token *current;

    current = tokens;
    while (current)
    {
        // 调试输出
        printf("处理token: [%s], 类型: %d\n", current->value, current->type);
        
        // 只对双引号和普通词token进行变量扩展
        if (current->type == TOKEN_DOUBLE_QUOTE || current->type == TOKEN_WORD)
        {
            char *expanded = expand_var_instr(current->value, env);
            if (expanded)
            {
                printf("扩展前: [%s], 扩展后: [%s]\n", current->value, expanded);
                free(current->value);
                current->value = expanded;
            }
        }
        // 单引号token保持原样，不进行变量扩展
        else if (current->type == TOKEN_SINGLE_QUOTE)
        {
            printf("单引号内容不扩展: [%s]\n", current->value);
        }
        
        current = current->next;
    }
    
    // 调试：打印最终token状态
    printf("\n扩展后的所有token:\n");
    current = tokens;
    while (current)
    {
        printf("Token: [%s], 类型: %d\n", current->value, current->type);
        current = current->next;
    }
}

// //===test extract_var_name ===
// int main()
// {
// 	char *str = "hi $USER_NAME bkdsk_dksekd";
// 	printf("extracted name: %s\n", extract_var_name(str));
// }