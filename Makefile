# executable name
NAME = minishell

# compiler & flags
CC = cc
# TODO clear up debugger
CFLAGS = -g -Wall -Wextra -Werror

# main src files
SRC_DIR = src
EXEC_DIR = $(SRC_DIR)/execution
BUILTIN_DIR = $(SRC_DIR)/builtin
MAIN_DIR = $(SRC_DIR)/main
SRC = $(MAIN_DIR)/shell_loop.c \
		$(MAIN_DIR)/shell_loop_utils.c \
		$(BUILTIN_DIR)/builtin_utils.c \
		$(BUILTIN_DIR)/ft_pwd.c \
		$(BUILTIN_DIR)/ft_cd.c \
		$(BUILTIN_DIR)/ft_echo.c \
		$(BUILTIN_DIR)/ft_env.c \
		$(BUILTIN_DIR)/ft_exit.c \
		$(BUILTIN_DIR)/ft_export.c \
		$(BUILTIN_DIR)/ft_unset.c

OBJ = $(SRC:.c=.o)

# readline
READLINE = -lreadline

# include directory
INC = -I./incl -I$(SRC_DIR)/libft

# libft directory
LIBFT_DIR = $(SRC_DIR)/libft
LIBFT = $(LIBFT_DIR)/libft.a

# rules
all: $(NAME)

# build libft first
$(LIBFT):
	make -C $(LIBFT_DIR)

$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(READLINE) -L$(LIBFT_DIR) -lft

%.o: %.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

clean:
	rm -f $(OBJ)
	make -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re