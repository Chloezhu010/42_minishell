/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop_util1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auzou <auzou@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 13:57:37 by auzou             #+#    #+#             */
/*   Updated: 2025/02/28 20:02:55 by auzou            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/loop.h"

t_token	*tokenize(char *input)
{
	t_token *tokens = NULL;
	int i = 0;
	int type = 0;

	while (input[i])
	{
		while (ft_isspace(input[i]))
			i++;
		if (is_special_char(input[i]))
		{
			char op[3] = {0};
			op[0] = input[i];
			if ((input[i] == '<' || input[i] == '>') && input[i + 1] == input[i])
				op[1] = input[++i];
			else if (input[i] == '&' && input[i + 1] == '&')
			{
				op[1] = '&';
				i++;
			}
			add_token(&tokens, create_token(op, get_token_type(op)));
			i++;
		}
		/* update the quote handler, extract_quoted also for double quoted str */
		else if (input[i] == '\'' || input[i] == '"')
		{
			char quote = input[i++];
			char *quoted = extract_quoted(input, &i, quote);
			if (!quoted)
				return (NULL);
			if (quote == '\'')
				type = TOKEN_SINGLE_QUOTE;
			else
				type = TOKEN_DOUBLE_QUOTE;
			add_token(&tokens, create_token(quoted, type));
			free(quoted);
		}
		// /* version origin */
		// else if (input[i] == '\'')
		// {
		// 	char quote = input[i++];
		// 	char *quoted = extract_quoted(input, &i, quote);
		// 	if (!quoted)
		// 		return (NULL);
		// 	add_token(&tokens, create_token(quoted, TOKEN_SINGLE_QUOTE));
		// 	free(quoted);
		// }
		// else if (input[i] == '"')
		// {
		// 	int start = i + 1;
		// 	while (input[i] && !ft_isspace(input[i]) && !is_special_char(input[i]))
		// 		i++;
		// 	char *word = strndup(&input[start], i - start - 1);
		// 	add_token(&tokens, create_token(word, TOKEN_WORD));
		// 	free(word);
		// }
		else
		{
			int start = i;
			while (input[i] && !ft_isspace(input[i]) && !is_special_char(input[i]))
				i++;
			char *word = strndup(&input[start], i - start);
			add_token(&tokens, create_token(word, TOKEN_WORD));
			free(word);
		}
	}
	return (tokens);
}

void	free_cmds(t_cmd *cmds)
{
	t_cmd	*tmp;
	int		i;

	while (cmds)
	{
		tmp = cmds;
		i = 0;
		while (cmds->args && cmds->args[i])
			free(cmds->args[i++]);
		free(cmds->args);
		free(cmds->infile);
		free(cmds->outfile);
		free(cmds->delimiter);
		cmds = cmds->next;
		free(tmp);
	}
}

t_cmd	*create_new_cmd(void)
{
	t_cmd	*cmd = calloc(1, sizeof(t_cmd));

	if (!cmd)
		return NULL;
	cmd->args = calloc(64, sizeof(char *));
	cmd->infile = NULL;
	cmd->outfile = NULL;
	cmd->append = 0;
	cmd->heredoc = 0;
	cmd->delimiter = NULL;
	cmd->fd_in = 0;
	cmd->next = NULL;
	return cmd;
}

t_cmd *parse_tokens(t_token *tokens)
{
    t_cmd *cmd_head = NULL;
    t_cmd *current_cmd = NULL;
    int i;

    if (!validate_tokens(tokens))
        return NULL;

    while (tokens)
    {
        if (!current_cmd)
        {
            current_cmd = create_new_cmd(); // 创建一个新的命令
            if (!cmd_head)
                cmd_head = current_cmd;
        }

        if (tokens->type == TOKEN_WORD
			|| tokens->type == TOKEN_SINGLE_QUOTE
			|| tokens->type == TOKEN_DOUBLE_QUOTE // add double quote
			|| tokens->type == TOKEN_COMMAND)
        {
            i = 0;
            while (current_cmd->args[i]) // 依次找到当前命令中空闲的位置
                i++;
            current_cmd->args[i] = strdup(tokens->value); // 将当前词添加到命令的参数中
        }
        else if (tokens->type == TOKEN_REDIRECT_IN
            || tokens->type == TOKEN_REDIRECT_OUT
            || tokens->type == TOKEN_REDIRECT_APPEND)
        {
            tokens = tokens->next;
            if (tokens)
            {
				if (tokens->type == TOKEN_REDIRECT_OUT)
                {
                    current_cmd->outfile = ft_strdup(tokens->value);
                    current_cmd->append = 0;
                }
                else if (tokens->type == TOKEN_REDIRECT_APPEND)
                {
                    current_cmd->outfile = ft_strdup(tokens->value);
                    current_cmd->append = 1;
                }
				/* add redirect in */
				else if (tokens->type == TOKEN_REDIRECT_IN)
				{
					current_cmd->infile = ft_strdup(tokens->value);
				}
            }
        }
        else if (tokens->type == TOKEN_PIPE) // 如果遇到管道符号，创建新的命令
        {
            if (tokens->next && (tokens->next->type == TOKEN_COMMAND || tokens->next->type == TOKEN_SINGLE_QUOTE))
            {
                current_cmd->next = create_new_cmd(); // 创建新的命令块
                current_cmd = current_cmd->next; // 设置当前命令为新的命令块
            }
        }
		/* add case for heredoc separately
			if the token after ">>" exits, meaning there is a delimiter
			- put heredoc as 1
			- append token value as the delimiter
		*/
		else if (tokens->type == TOKEN_HEREDOC)
		{
			tokens = tokens->next;
			if (tokens)
			{
				current_cmd->heredoc = 1;
				current_cmd->delimiter = ft_strdup(tokens->value);
			}
		}
        tokens = tokens->next;
    }

    return cmd_head;
}

void	check_format_command(t_token *tokens)
{
	while (tokens)
	{
		if (tokens->type == TOKEN_WORD)
		{
			if ((strcmp(tokens->value, "cd") == 0)
				|| (strcmp(tokens->value, "ls") == 0)
				|| (strcmp(tokens->value, "cp") == 0)
				|| (strcmp(tokens->value, "mv") == 0)
				|| (strcmp(tokens->value, "rm") == 0)
				|| (strcmp(tokens->value, "mkdir") == 0)
				|| (strcmp(tokens->value, "rmdir") == 0)
				|| (strcmp(tokens->value, "man") == 0)
				|| (strcmp(tokens->value, "more") == 0)
				|| (strcmp(tokens->value, "cat") == 0)
				|| (strcmp(tokens->value, "echo") == 0)
				|| (strcmp(tokens->value, "grep") == 0)
				|| (strcmp(tokens->value, "pwd") == 0)
				|| (strcmp(tokens->value, "export") == 0)
				|| (strcmp(tokens->value, "unset") == 0)
				|| (strcmp(tokens->value, "env") == 0)
				|| (strcmp(tokens->value, "exit") == 0))
				tokens->type = TOKEN_COMMAND;
		}
		tokens = tokens->next;
	}
}

// void	print_cmds(t_cmd *cmd)
// {
// 	int	i;
// 	i = 0;
// 	while (cmd)
// 	{
// 		printf("\nCommand:\n");
// 		while (cmd->args && cmd->args[i])
// 		{
// 			printf("  Arg[%d]: %s\n", i, cmd->args[i]);
// 			i++;
// 		}
// 		if (cmd->infile)
// 			printf("  Input Redirect: %s\n", cmd->infile);
// 		if (cmd->outfile)
// 			printf("  Output Redirect: %s (%s)\n",
// 				cmd->outfile, cmd->append ? "append" : "overwrite");
// 		cmd = cmd->next;
// 		i = 0;
// 	}
// }

// void	print_tokens(t_token *tokens)
// {
// 	while (tokens)
// 	{
// 		printf("Token: %-10s | Type: %d\n", tokens->value, tokens->type);
// 		tokens = tokens->next;
// 	}
// }

// int	main(void)
// {
// 	char	*input;
// 	t_token	*tokens;
// 	t_cmd	*cmds;
// 	while (1){
// 		input = readline("$>");
// 		tokens = tokenize(input);
// 		if (tokens != NULL)
// 		{
// 			expand_tokens(tokens);
// 			check_format_command(tokens);
// 			print_tokens(tokens);
// 			cmds = parse_tokens(tokens);
// 			if (cmds)
// 				print_cmds(cmds);
// 		}
// 		if (tokens != NULL)
// 			free_tokens(tokens);
// 		if (cmds != NULL)
// 			free_cmds(cmds);
// 	}
// 	return (0);
// }
// // === test expand_token ===
// int main(int ac, char **av)
// {
// 	(void)ac;
// 	printf("%s\n", expand_variable(av[1]));
// }

// // === test tokenize() ===
// void test_tokenize(char *input)
// {
// 	printf("test input: %s\n", input);
// 	t_token *tokens = tokenize(input);
// 	if (!tokens)
// 	{
// 		printf("no token generated\n");
// 		return ;
// 	}
// 	t_token *current = tokens;
// 	while (current)
// 	{
// 		printf("token: %s, type: %d\n", current->value, current->type);
// 		current = current->next;
// 	}
// 	free_tokens(tokens);
// }
// // === test tokenize() main function ===
// int main(void)
// {
// 	test_tokenize("echo \"hi $USER\"");
//     test_tokenize("ls -l");
//     test_tokenize("echo 'Hello World'");
//     test_tokenize("ls > output.txt");
//     test_tokenize("ls | grep .c");
//     test_tokenize("cat << EOF");
//     test_tokenize("");
//     test_tokenize("   ");
//     test_tokenize("echo 'Hello World' > output.txt | cat");
//     return 0;
// }