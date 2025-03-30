/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_utils3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czhu <czhu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 17:59:13 by auzou             #+#    #+#             */
/*   Updated: 2025/03/25 12:48:10 by czhu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/loop.h"

void tokenize_util4(t_token **tokens, int *i, char *input)
{
    int     start;
    char    *word;
    t_token *new_token;
    int     is_consecutive = 0;

    start = *i;
    while (input[*i] && !ft_isspace(input[*i]) && !is_special_char(input[*i]) 
           && !is_quote(input[*i]))
        (*i)++;
    
    if (*i > start)
    {
        word = strndup(&input[start], *i - start);
        
        // 检查下一个字符是否是引号或者其他非空格字符
        // 如果是，则标记为连续
        if (input[*i] && (is_quote(input[*i]) || (!ft_isspace(input[*i]) && !is_special_char(input[*i]))))
        {
            is_consecutive = 1;
        }
        
        new_token = create_token(word, TOKEN_WORD);
        new_token->consecutive_quote = is_consecutive;
        add_token(tokens, new_token);
        free(word);
    }
}
