# executable name
NAME = minishell

# compiler & flags
CC = cc
# TODO clear up debugger
CFLAGS = -g -Wall -Wextra -Werror

VAL_FLAGS = --leak-check=full --show-leak-kinds=all --track-origins=yes --track-fds=yes --suppressions=supp.supp

# main src files
SRC_DIR = src
EXEC_DIR = $(SRC_DIR)/execution
BUILTIN_DIR = $(SRC_DIR)/builtin
MAIN_DIR = $(SRC_DIR)/main
UTILS_DIR = $(SRC_DIR)/utils
SIG_DIR = $(SRC_DIR)/signal
PIPE_DIR = $(SRC_DIR)/pipe
EXPAND_DIR = $(SRC_DIR)/expander
REDIRECT_DIR = $(SRC_DIR)/redirect
SRC = $(BUILTIN_DIR)/builtin_utils.c \
		$(BUILTIN_DIR)/init_builtin.c \
		$(BUILTIN_DIR)/ft_pwd.c \
		$(BUILTIN_DIR)/ft_cd.c \
		$(BUILTIN_DIR)/ft_echo.c \
		$(BUILTIN_DIR)/ft_env.c \
		$(BUILTIN_DIR)/ft_exit.c \
		$(BUILTIN_DIR)/ft_export.c \
		$(BUILTIN_DIR)/ft_unset.c \
		$(UTILS_DIR)/shell_loop_utils.c \
		$(UTILS_DIR)/loop_util1.c \
		$(UTILS_DIR)/loop_util2.c \
		$(UTILS_DIR)/redir_utils.c \
		$(UTILS_DIR)/cmd_utils.c \
		$(UTILS_DIR)/parse_tokens_utils.c \
		$(UTILS_DIR)/token_utils_1.c \
		$(UTILS_DIR)/token_utils_2.c \
		$(UTILS_DIR)/tokenize_utils3.c \
		$(UTILS_DIR)/tokenize_utils1.c \
		$(UTILS_DIR)/tokenize_utils2.c \
		$(UTILS_DIR)/execute_shell_utils.c \
		$(UTILS_DIR)/extract_quoted_utils.c \
		$(MAIN_DIR)/shell_loop.c \
		$(MAIN_DIR)/main.c \
		$(SIG_DIR)/signal.c \
		$(SIG_DIR)/signal_utils.c \
		$(EXEC_DIR)/execute_path.c \
		$(EXEC_DIR)/execute_utils.c \
		$(EXPAND_DIR)/expander.c \
		$(EXPAND_DIR)/expander_utils.c \
		$(PIPE_DIR)/pipe.c \
		$(PIPE_DIR)/pipe_utils1.c \
		$(PIPE_DIR)/pipe_utils2.c \
		$(PIPE_DIR)/pipe_utils3.c \
		$(PIPE_DIR)/pipe_utils4.c \
		$(REDIRECT_DIR)/redirect.c \
		$(REDIRECT_DIR)/redirect_utils1.c \
		$(REDIRECT_DIR)/redirect_utils2.c 

OBJ = $(SRC:.c=.o)

# readline
READLINE_INC = -I/opt/homebrew/opt/readline/include
READLINE_LIB = -L/opt/homebrew/opt/readline/lib -lreadline -lhistory


# include directory
INC = -I./incl -I$(SRC_DIR)/libft $(READLINE_INC)

# libft directory
LIBFT_DIR = $(SRC_DIR)/libft
LIBFT = $(LIBFT_DIR)/libft.a

# rules
all: $(NAME)

# build libft first
$(LIBFT):
	make -C $(LIBFT_DIR)

$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(READLINE_LIB) -L$(LIBFT_DIR) -lft

%.o: %.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

clean:
	rm -f $(OBJ)
	make -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	make -C $(LIBFT_DIR) fclean

valgrind: $(NAME)
	@valgrind $(VAL_FLAGS) ./$(NAME)

re: fclean all

.PHONY: all clean fclean re