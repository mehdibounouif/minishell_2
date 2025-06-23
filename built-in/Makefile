NAME = minishell
CC = gcc
CFLAGS = -Wall -Wextra -Werror -I../includes
LDFLAGS = -lreadline
LIBFT_DIR = ../libft
LIBFT = $(LIBFT_DIR)/libft.a

SRCS = test_builtin.c \
       cd_command.c \
	   	echo_command.c \
		

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(LIBFT):
	@make -C $(LIBFT_DIR)

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(LDFLAGS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)
	@make -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	@make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
