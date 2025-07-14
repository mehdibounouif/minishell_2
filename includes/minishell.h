/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moraouf <moraouf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 07:55:09 by mbounoui          #+#    #+#             */
/*   Updated: 2025/07/14 10:19:53 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include "../libft/libft.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdbool.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>

#define EMPTY 0
#define PIPE 1
#define WORD 2
#define R_OUT 3
#define R_IN 4
#define R_APPEND 5
#define HEREDOC 9

// TYPES OF NODES

#define COMMAND_NODE 10
#define PIPE_NODE 11
#define REDIRECT_NODE 12
#define END_NODE 13

#define HEREDOC_FILE ".heredoc_file_"
extern int ret;

typedef struct s_command t_command;
typedef struct s_redirection t_redirection;
typedef struct s_tree t_tree;
typedef struct s_node t_node;
typedef struct s_pipe t_pipe;
typedef struct s_end t_end;
typedef struct s_env t_env;
typedef struct s_mini t_mini;
typedef struct s_herdoc t_herdoc;

struct s_env {
	char *key;
	char *value;
	struct s_env *next;
};

struct s_node {
	int	quoted;
	int type;
	char *content;
	struct s_node *next;
	struct s_node *prev;
};

struct s_pipe {
  t_tree *left;
  t_tree *right;
};

struct s_command {
  char *command;
  char **args;
};

struct	s_herdoc {
	char	*herdoc;
	char	*delimeter;
	int	quoted;
	int fd;
	struct s_herdoc *next;
};

struct s_redirection {
	t_tree *prev; // previous node;
	char *in_file; // last input file;
	char	**in_files; // all input files;
	char *out_file; // last output file;
	char	**out_files; // all output file;
	int	*heredoc_fds; // heredoc fds;
	int	in; // flag to know if the last input file is the stdin or not;
	int	hdc; // flag to know if the last herdoc file is the stdin or not;
	int in_count; // number of input files;
	int out_count; // number of output files;
	int	out_type; // type of output file (trunk or append);
	int last_fd; // last heredoc fd;
	t_herdoc *herdoc; // list of command heredocs;
};

struct s_tree {
  int type;
  t_command *command;
  t_pipe *pipe;
  t_redirection *redirect;
};

struct s_mini {
  t_tree *tree;
  t_env *env;
  t_node *list;
  int	ret;
};


// built-in functions
// Function declarations
int cd_command(t_env *env, char **args);
int echo_command(t_env *env, char **args);
int pwd_command(t_env *env, char **args);
int export_command(t_env *env, char **args);
int unset_command(t_env *env, char **args);
int env_command(t_env *env, char **args);
int exit_command(t_env *env, char **args);
char *get_env_value(t_env *env, char *key);
int update_env_var(t_env *env, char *var);
t_env *init_env(char **envp);
void set_env_var(t_env **env, char *key, char *value);
void free_env(t_env *env);
void add_env_var(t_env **env, char *key, char *value);
void sorted_env(t_env *env);
void	ft_lstadd_node(t_env **lst, t_env *node);
t_env *create_env_var(char *key, char *value);
t_env *get_env_var(t_env *env, const char *key);
void  get_env(t_mini *minishell, char **env);


// Command execution
void execute_command(t_env *env, const char *input);
void	execute_pipe_node(t_tree *tree, t_env *env, char **envp);
void print_env(t_env *env);
void print_tree_unicode(t_tree *tree, const char *prefix, int is_last);
void	open_herdocs(t_tree *tree, t_env *env);
void	print_her(t_herdoc *herdoc);
void	print_data(t_redirection *node);
void	print_redirection_data(t_tree *tree);

// Tokenize
int tokenize(char *cmd, t_node **list);
int             check_real_sep(char *line, int i);
t_node  *get_token(char *line, int *i);
int             calc_token_byte(char *line, int *i);
void    token_type(t_node *node, int flag);
int     ft_strcmp(const char *s1, const char *s2);


int is_space(char c);
void add_back(t_node **list, t_node *node);
void add_back2(t_env **list, t_env *node);
void	add_back3(t_herdoc **list, t_herdoc *node);
void print_list(t_node *list);


// AST TREE
t_tree *pars_command(t_node **list, t_env *env);
t_tree	*parss_one_side(t_node **list, t_env *env);
t_tree  *parss_redirection(t_tree *node, t_node **list, t_env *env);
t_tree	*parss_redirection_in_start(t_node **list, t_env *env);
//t_tree	*parss_herdoc(t_tree *node, t_node **list, t_env *env);
t_tree  *command_without_redirection(t_node **list);
t_tree  *pars_pipe(t_node **list, t_env *env);
t_tree	*parss_herdoc(t_tree *node, t_node *list, t_env *env);
void	collect_herdoc(t_tree *node, t_node *list);
char	*get_last_herdoc(t_herdoc *list);
char	*get_last_file(char **list);


void	free_tree(t_tree **tree);
void free_str(char **list);
void free_list(t_node **list);
void ft_free(t_mini *minishell);
int check_quotes(char *cmd, size_t i);
char	*remove_quotes(char *cmd);
char    *remove_quotes2(char *cmd, size_t l);
char	*remove_quotes3(char *cmd);
int	is_quoted(char *cmd, int len);
int is_separator(char *line, int i);
int	is_empty(char *s);
int	is_space(char c);
int is_real_separator(char *cmd, int i);
int count_args(t_node *list);
int check_sides(t_node *list);
t_tree *parssing_line(char *cmd, t_mini *minishell);
int readline_and_parssing(t_mini *minishell);
int     check_syntax(t_mini *mini, t_node *list);
int	is_redirection(t_node *node);

// EXPANSION
int	is_dollar(char *cmd, int i);
char	*get_env_key(char *cmd, int i);
char	*expansion(char *cmd, t_env *list);
/*
char    *get_special_value(char *key, int ret);
int	get_special_len(char *cmd, int i);
char    *check_square_bracket_alpha(char *key, int i);
char    *check_square_bracket_digit(char *key, int i);
char    *check_square_bracket(char *key);
char    *check_curly_brace(char *key);
*/

// EXECUTE 
int execute_full_command(t_tree *node, t_env *env, char **envp);
void execute_command_node(t_tree *node, t_env *env, char **envp);
void	execute_redirection_command(t_tree *node, t_env *env, char **envp);
char	*ft_getenv(char *key, t_env *list);
char	*find_path(t_tree *node, t_env *list);


//SIGNALS 

// int   g_ctrl_c; 
void handle_signal();


#endif
