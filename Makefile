NAME = minishell
RM = rm -f
CC = cc
FLAGS = -Wall -Wextra -Werror -g
LIBFT_DIR = ./libft
LIBFT_LIB = $(LIBFT_DIR)/libft.a
SRCS_DIR1 = ./parssing
SRCS_DIR2 = ./execution
SRCS_DIR3 = ./built-in
SRCS_FILES1 = main.c\
	     utils.c\
	     token/tokenize.c\
	     token/tools.c\
	     ast_tree/tree.c\
	     ast_tree/print_ast.c\
	     ast_tree/command_with_redirection.c\
	     ast_tree/command_with_herdoc.c\
	     ast_tree/command_with_pipe.c\
		 ast_tree/simple_command.c\
	     check_quotes.c\
	     free.c\
	     check_command.c\
	     check_syntax.c\
	     expaned/expansions.c\
	     expaned/special_dollar.c\
	     expaned/tools.c\

SRCS_FILES2 = execute_simple_command.c\
	      execute_pipe_command.c\
	      execute_redirection_command.c\
	      execution.c\
	      signals.c\
	      utls.c\

SRCS_FILES3 = cd_command.c\
	      echo_command.c\
	      pwd_command.c\
	      export_command.c\
	      export_utils.c\
	      exit_command.c\
	      unset_command.c\
	      env_command.c\
	      env_utils.c\

SRCS1 = $(addprefix $(SRCS_DIR1)/, $(SRCS_FILES1))
SRCS2 = $(addprefix $(SRCS_DIR2)/, $(SRCS_FILES2))
SRCS3 = $(addprefix $(SRCS_DIR3)/, $(SRCS_FILES3))

PARSS_OBJS = $(SRCS1:.c=.o)
EXEC_OBJS = $(SRCS2:.c=.o)
BUILTIN_OBJS = $(SRCS3:.c=.o)

all: $(NAME)

$(LIBFT_LIB):
	@$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(PARSS_OBJS) $(EXEC_OBJS) $(BUILTIN_OBJS) $(LIBFT_LIB)
	@echo "🎉 Vincular archivos de objeto para crear el ejecutable: $(NAME) 🚀"
	@$(CC) $(FLAGS) $(PARSS_OBJS) $(EXEC_OBJS) $(BUILTIN_OBJS) $(LIBFT_LIB) -lreadline -o $(NAME)
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
	@$(RM) $(BUILTIN_OBJS)
	@$(RM) .h*
	@echo "✅ Limpieza completa."

fclean: clean
	@echo "🧹 Limpieza de archivos de objetos bibliotecas (.a) y ejecutables ..."
	@$(MAKE) fclean -C $(LIBFT_DIR)
	@$(RM) $(NAME)
	@echo "✅ Limpieza completa."

re: fclean all

.SECONDARY: $(PARS_OBJS) $(EXEC_OBJS) $(BUILTIN_OBJS)

.PHONY: re all clean fclean
