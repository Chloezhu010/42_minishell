/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czhu <czhu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 16:23:00 by czhu              #+#    #+#             */
/*   Updated: 2025/03/29 16:39:25 by czhu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOOP_H
# define LOOP_H

# include "../incl/minishell.h"

# define TOKEN_WORD 0
# define TOKEN_PIPE 1
# define TOKEN_OR 11
# define TOKEN_REDIRECT_IN 2
# define TOKEN_REDIRECT_OUT 3
# define TOKEN_REDIRECT_APPEND 4
# define TOKEN_HEREDOC 5
# define TOKEN_SINGLE_QUOTE 6
# define TOKEN_DOUBLE_QUOTE 10
# define DIRECTORY '/'
# define TOKEN_AND 7
# define TOKEN_AND_SINGLE 8
# define TOKEN_COMMAND 9

typedef struct s_env	t_env;
typedef struct s_token
{
	char			*value;
	int				type;
	int				consecutive_quote;
	struct s_token	*next;
}	t_token;

typedef struct s_redir
{
	char			*file;
	int				type;
	struct s_redir	*next;
}	t_redir;

typedef struct s_cmd
{
	char			**args;
	char			*infile;
	char			*outfile;
	int				append;
	int				heredoc;
	char			*delimiter;
	int				fd_in;
	t_redir			*redirects;
	int				in_pipe;
	struct s_cmd	*next;
}	t_cmd;

t_token	*tokenize(char *input);
void	extract_redirect_filename(char *input, int *i, t_token **tokens);
void	free_cmds(t_cmd *cmds);
t_cmd	*create_new_cmd(void);
t_cmd	*parse_tokens(t_token *tokens);
void	check_format_command(t_token *tokens);

/* redirect */
t_redir	*create_new_redir(char *file, int type);
void	update_cmd_files(t_cmd *cmd, char *file, int type);
void	add_redir(t_cmd *cmd, char *file, int type);
void	free_redir(t_redir *redir);

/* tokenize_utils1 */
int		ft_isspace(char a);
int		is_special_char(char c);
int		is_quote(char c);
void	tokenize_util1(t_token **tokens, int *i, char *input);
void	tokenize_util2(t_token **tokens, int *i, char *input);
int		tokenize_util3(t_token **tokens, int *i, char *input);
void	tokenize_util4(t_token **tokens, int *i, char *input);

/* token_utils_1 */
void	add_token(t_token **head, t_token *new_token);
char	*extract_quoted(char *input, int *i, char quote);
t_token	*create_token(char *value, int type);
int		get_token_type(char *s);
void	free_tokens(t_token *tokens);

/* token_utils_2 */
int		check_format_of_pipe(t_token *tokens);
int		check_format_of_redirect(t_token *tokens);
int		validate_tokens(t_token *tokens);

/* parse_tokens_utils*/
t_cmd	*create_new_command(t_cmd **cmd_head);
void	process_token_word(t_cmd *current_cmd, t_token *tokens);
void	process_redirection(t_cmd *current_cmd, t_token **tokens, int type);
void	process_heredoc(t_cmd *current_cmd, t_token **tokens);

/* extract_quoted_utils*/
char	*extract_single_quoted(char *input, int *i, char quote);
char	*process_diff_quote(char *input, int *i, char quote, char *current);
char	*handle_consecutive_quote(char *input,
			int *i, char quote, char *current);
char	*process_additional_quotes(char *input, int *i, char *quoted);

#endif
