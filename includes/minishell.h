/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 07:55:09 by mbounoui          #+#    #+#             */
/*   Updated: 2025/05/30 09:44:07 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include "../libft/libft.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define PIPE 1
#define WORD 2
#define R_OUT 3
#define R_IN 4
#define R_APPEND 5
#define R_ERR 6
#define END 7
#define OR 8
#define HEREDOC 9

// TYPES OF NODES

#define COMMAND_NODE 10
#define PIPE_NODE 11
#define REDIRECT_NODE 12
#define END_NODE 13

typedef struct s_command t_command;
typedef struct s_redirection t_redirection;
typedef struct s_tree t_tree;
typedef struct s_node t_node;
typedef struct s_pipe t_pipe;
typedef struct s_end t_end;
typedef struct s_env t_env;
typedef struct s_mini t_mini;

struct s_env {
  char *key;
  char *value;
  struct s_env *next;
};

struct s_node {
  int type;
  char *content;
  struct s_node *next;
  struct s_node *prev;
};

struct s_pipe {
  t_tree *left;
  t_tree *right;
};

struct s_end {
  t_tree *left;
  t_tree *right;
};

struct s_command {
  char *command;
  char **args;
};

struct s_redirection {
  t_tree *prev;
  char *file;
  char *redirect;
  int redirection_type;
};

struct s_tree {
  int type;
  t_command *command;
  t_pipe *pipe;
  t_pipe *end;
  t_redirection *redirect;
};

struct s_mini {
  t_tree *tree;
  t_env *env;
  t_node *list;
};

// built-in functions
// Function declarations
int cd_command(t_env *env, char **args);
char *get_env_value(t_env *env, char *key);
int update_env_var(t_env *env, char *var);
t_env *init_env(char **envp);
void free_env(t_env *env);
void add_env_var(t_env **env, char *key, char *value);
// Command execution
void execute_command(t_env *env, const char *input);

void print_env(t_env *env);
void print_tree_unicode(t_tree *tree, const char *prefix, int is_last);
int tokenize(char *command, t_node **list);
int is_space(char c);
void add_back(t_node **list, t_node *node);
void add_back2(t_env **list, t_env *node);
void print_list(t_node *list);
t_tree *pars_command(t_node **list);
void free_str(char **list);
void free_list(t_node **list);
void ft_free(t_mini *minishell);
int check_quotes(char *cmd, size_t i);
char *remove_quotes(char *cmd);
int is_separator(char c);
int is_real_separator(char *cmd, int i);
int count_args(t_node *list);
int check_sides(t_node *list);
t_tree *parssing_line(char *cmd, t_mini *minishell);
int readline_and_parssing(t_mini *minishell, char **env);
int	check_syntax(t_node *list);
int	is_redirection(t_node *node);

// EXECUTE 
int execute_full_command(t_tree *node, char **env);
int execute_command_node(t_tree *node, char **env);
char	*ft_getenv(char *key, char **env);
char	*find_path(t_tree *node, char **env);
#endif
