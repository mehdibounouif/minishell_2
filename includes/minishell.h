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

#include "../libft/libft.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>

#define PIPE 1
#define WORD 2
#define REDIRECTION_OUT 3
#define REDIRECTION_IN 4
#define REDIRECTION_APPEND 5
#define REDIRECTION_ERR 6
#define AND 7
#define OR 8
#define HEREDOC 9

// TYPES OF NODES

#define COMMAND_NODE 1
#define PIPE_NODE 2
#define REDIRECT_NODE 3
#define OR_NODE 4
#define AND_NODE 5

typedef struct s_command t_commaned;
typedef struct s_redirection t_redirection;
typedef struct s_tree t_tree;
typedef struct s_node t_node;
typedef struct s_pipe t_pipe;
typedef struct s_and t_and;
typedef struct s_or t_or;

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

struct s_and {
  t_tree *left;
  t_tree *right;
};

struct s_or {
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
  t_commaned *command;
  t_pipe *pipe;
  t_or *orr;
  t_and *andd;
  t_redirection *redirect;
};

void print_tree_unicode(t_tree *tree, const char *prefix, int is_last);
void tokenize(char *command, t_node **list);
int is_space(char c);
void add_back(t_node **list, t_node *node);
void print_list(t_node *list);
t_tree *pars_command(t_node **list);
void ft_free(t_node **list, t_tree **tree);
int check_quotes(char *cmd, int i);
char *remove_quotes(char *cmd);
