/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop_util1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czhu <czhu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 13:57:37 by auzou             #+#    #+#             */
/*   Updated: 2025/03/23 13:52:53 by czhu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/loop.h"
#include "../../incl/minishell.h"

/* helper function to extract a filename that follows a redirect ops */
void	extract_redirect_filename(char *input, int *i, t_token **tokens)
{
	int		start;
	char	*word;

	start = *i;
	while (input[*i] && !ft_isspace(input[*i])
		&& !is_quote(input[*i]) && input[*i] != '|')
		(*i)++;
	if (*i > start)
	{
		word = strndup(&input[start], *i - start);
		add_token(tokens, create_token(word, TOKEN_WORD));
		free(word);
	}
}

t_token	*tokenize(char *input)
{
	t_token	*tokens;
	int		i;

	tokens = NULL;
	i = 0;
	while (input[i])
	{
		while (ft_isspace(input[i]))
			i++;
		if (is_special_char(input[i]))
			tokenize_util1(&tokens, &i, input);
		else if (input[i] == '|')
			tokenize_util2(&tokens, &i, input);
		else if (input[i] == '\'' || input[i] == '"')
		{
			if (tokenize_util3(&tokens, &i, input) == 1)
				return (NULL);
		}
		else
		{
			tokenize_util4(&tokens, &i, input);
		}
	}
	return (tokens);
}

void	add_redir(t_cmd *cmd, char *file, int type)
{
	t_redir	*new_redir;
	t_redir	*temp;

	new_redir = create_new_redir(file, type);
	if (!new_redir)
		return ;
	update_cmd_files(cmd, file, type);
	if (!cmd->redirects)
		cmd->redirects = new_redir;
	else
	{
		temp = cmd->redirects;
		while (temp->next)
			temp = temp->next;
		temp->next = new_redir;
	}
}

t_cmd	*parse_tokens(t_token *tokens)
{
	t_cmd	*cmd_head;
	t_cmd	*current_cmd;

	cmd_head = NULL;
	current_cmd = NULL;
	if (!validate_tokens(tokens))
		return (NULL);
	while (tokens)
	{
		if (!current_cmd)
			current_cmd = create_new_command(&cmd_head);
		if (tokens->type == TOKEN_WORD || tokens->type == TOKEN_SINGLE_QUOTE
			|| tokens->type == TOKEN_DOUBLE_QUOTE
			|| tokens->type == TOKEN_COMMAND)
			process_token_word(current_cmd, tokens);
		else if (tokens->type == TOKEN_REDIRECT_OUT)
			process_redirection(current_cmd, &tokens, TOKEN_REDIRECT_OUT);
		else if (tokens->type == TOKEN_REDIRECT_APPEND)
			process_redirection(current_cmd, &tokens, TOKEN_REDIRECT_APPEND);
		else if (tokens->type == TOKEN_REDIRECT_IN)
			process_redirection(current_cmd, &tokens, TOKEN_REDIRECT_IN);
		else if (tokens->type == TOKEN_PIPE)
		{
			if (tokens->next)
			{
				current_cmd->next = create_new_cmd();
				current_cmd = current_cmd->next;
			}
		}
		else if (tokens->type == TOKEN_HEREDOC)
			process_heredoc(current_cmd, &tokens);
		tokens = tokens->next;
	}
	return (cmd_head);
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
// 	test_tokenize("cat <minishell.h|ls");
// 	test_tokenize("cat <minishell.h>./outfiles/outfile");
// 	test_tokenize("cat < \"output.txt\"");
// 	// test_tokenize("cat \"output.txt\"");
// 	// test_tokenize("cat output.txt");
// 	// test_tokenize("echo \"hi $USER\"");
//     // test_tokenize("ls -l");
//     // test_tokenize("echo 'Hello World'");
//     // test_tokenize("ls > output.txt");
//     // test_tokenize("ls | grep .c");
//     // test_tokenize("cat << EOF");
//     // test_tokenize("");
//     // test_tokenize("   ");
//     // test_tokenize("echo 'Hello World' > output.txt | cat");
//     return 0;
// }
