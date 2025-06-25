NAME = minishell
RM = rm -f
CC = cc
FLAGS = -Wall -Wextra -Werror -g
LIBFT_DIR = ./libft
LIBFT_LIB = $(LIBFT_DIR)/libft.a
SRCS_DIR1 = ./parssing
SRCS_DIR2 = ./execution
SRCS_FILES1 = main.c\
	     utils.c\
	     tokenize.c\
	     AST.c\
	     print_ast.c\
	     check_quotes.c\
	     free.c\
	     check_command.c\
	     check_syntax.c\
	     expansions.c\

SRCS_FILES2 = execute_simple_command.c\
	      execution.c\
		  signals.c\
	      utls.c\

SRCS1 = $(addprefix $(SRCS_DIR1)/, $(SRCS_FILES1))
SRCS2 = $(addprefix $(SRCS_DIR2)/, $(SRCS_FILES2))

PARSS_OBJS = $(SRCS1:.c=.o)
EXEC_OBJS = $(SRCS2:.c=.o)

all: $(NAME)

$(LIBFT_LIB):
	@$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(PARSS_OBJS) $(EXEC_OBJS) $(LIBFT_LIB)
	@echo "🎉 Vincular archivos de objeto para crear el ejecutable: $(NAME) 🚀"
	@$(CC) $(FLAGS) $(PARSS_OBJS) $(EXEC_OBJS) $(LIBFT_LIB) -lreadline -o $(NAME)
	@echo "✅ ¡Compilación exitosa! Ejecutable creado.: $(NAME)"

%.o: %.c
	@echo "🔨 Compilando ..."
	@$(CC) $(FLAGS) -o $@ -c $<
	@echo "✅ ¡Compilado exitosamente!"

clean:
	@echo "🧹 Limpieza de archivos de objetos ..."
	@$(MAKE) clean -C $(LIBFT_DIR)
	@$(RM) $(PARSS_OBJS)
	@$(RM) $(EXEC_OBJS)
	@echo "✅ Limpieza completa."

fclean: clean
	@echo "🧹 Limpieza de archivos de objetos bibliotecas (.a) y ejecutables ..."
	@$(MAKE) fclean -C $(LIBFT_DIR)
	@$(RM) $(NAME)
	@echo "✅ Limpieza completa."

re: fclean all

.SECONDARY: $(OBJS)

.PHONY: re all clean fclean
