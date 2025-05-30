NAME = minishell
RM = rm -f
CC = cc
FLAGS = -Wall -Wextra -Werror -fsanitize=address -g
LIBFT_DIR = ./libft
LIBFT_LIB = $(LIBFT_DIR)/libft.a
SRCS_DIR = ./parssing
SRCS_FILES = main.c\
			 utils.c\
			 tokenize.c\
			 AST.c\
			 print_ast.c\
			 #free.c\

SRCS = $(addprefix $(SRCS_DIR)/, $(SRCS_FILES))

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(LIBFT_LIB):
	@$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(OBJS) $(LIBFT_LIB)
	@echo "🎉 Vincular archivos de objeto para crear el ejecutable: $(NAME) 🚀"
	@$(CC) $(FLAGS) -lreadline $(OBJS) $(LIBFT_LIB) -o $(NAME)
	@echo "✅ ¡Compilación exitosa! Ejecutable creado.: $(NAME)"

%.o: %.c
	@echo "🔨 Compilando ..."
	@$(CC) $(FLAGS) -o $@ -c $<
	@echo "✅ ¡Compilado exitosamente!"

clean:
	@echo "🧹 Limpieza de archivos de objetos ..."
	@$(MAKE) clean -C $(LIBFT_DIR)
	@$(RM) $(OBJS)
	@echo "✅ Limpieza completa."

fclean: clean
	@echo "🧹 Limpieza de archivos de objetos bibliotecas (.a) y ejecutables ..."
	@$(MAKE) fclean -C $(LIBFT_DIR)
	@$(RM) $(NAME)
	@echo "✅ Limpieza completa."


re: fclean all

.SECONDARY: $(OBJS)

.PHONY: re all clean fclean
