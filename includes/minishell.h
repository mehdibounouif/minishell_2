/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taha_laylay <taha_laylay@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 08:10:15 by mbounoui          #+#    #+#             */
/*   Updated: 2025/08/10 00:46:11 by taha_laylay      ###   ########.fr       */
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
#include <sys/stat.h>
#include <signal.h>
#include <sys/wait.h>
#include <dirent.h>

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
typedef struct s_files t_files;

struct s_env {
	char *key;
	char *value;
	struct s_env *next;
};

struct s_node {
	int	contain_quoted;
	int	between_quoted;
	int type;
	int b_space;
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
  char  *file;
	char	*herdoc;
	char	*delimeter;
	int	quoted;
//	int fd;
	struct s_herdoc *next;
};

struct	s_files
{
	int	type;
	char *file;
	struct s_files *next;
};

struct s_redirection
{
	t_tree *prev; // previous node;
	char *in_file; // last input file;
	char	**in_files; // all input files;
	char *out_file; // last output file;
	char	**out_files; // all output file;
	char	**heredocs;
	char *hrc_file; // last output file;
	int	in; // flag to know if the last input file is the stdin or not;
	int	hdc; // flag to know if the last herdoc file is the stdin or not;
	int in_count; // number of input files;
	int out_count; // number of output files;
	int	out_type; // type of output file (trunk or append);
	t_files *files;
	t_herdoc *herdoc; // list of command heredocs;
};

struct s_tree {
	int type;
	int ret;
	t_command *command;
	t_pipe *pipe;
	t_redirection *redirect;
};

struct s_mini {
	t_tree *tree;
	t_node *list;
};

typedef struct s_gcollect
{
	void				*value;
	struct s_gcollect	*next;
}	t_gcollect;

// expantion struct
typedef	struct t_share
{
	int	i;
	int	j;
	int	h;
	char	*expanded_cmd;
}	t_share;


// built-in functions
// Function declarations
int cd_command(t_env *env, char **args);
int echo_command(char **args);
int pwd_command();
int export_command(t_env **env, char **args);
int unset_command(t_env **env, char **args);
int env_command(t_env *env, char **args);
int	exit_command(t_tree *node, t_env *env, char **args);
char *get_env_value(t_env *env, char *key);
int update_env_var(t_env *env, char *var);
void set_env_var(t_env **env, char *key, char *value);
void free_env(t_env *env);
void add_env_var(t_env **env, char *key, char *value);
void sorted_env(t_env *env);
void	ft_lstadd_node(t_env **lst, t_env *node);
t_env *create_env_var(char *key, char *value);
t_env *get_env_var(t_env *env, const char *key);
int  get_env(t_env **envp, char **env);
void	print_env_export(t_env *env);

// Custom_libft
char	*ft_strjoin1(char const *s1, char const *s2);
char	*ft_substr1(char const *s, unsigned int start, size_t len);
char	*ft_strdup1(const char *s);
char	**ft_split1(char const *s, char c);

// Command execution
// PIPE
void	execute_pipe_node(t_tree *tree, t_env *env, char **envp);
void print_env(t_env *env);
void print_ast(t_tree *tree, int level);
int	open_herdocs(int *flag, t_tree *tree, t_env *env);
void	skip_redirection(t_node **list);
char **back_up();
void	print(char *command, char *message, int code);
int	execute_builtin(t_tree *node, t_env **env);
int	is_builtin(char *command);
int	execute_builtin_command(char *command, char **args, t_env **env);
void	dup_fds(t_redirection *node, t_env *env);
// void	dup_output(t_redirection *node);
// void	dup_input(t_redirection *node);

// Tokenize
void	tokenize(char *cmd, t_node **list);
size_t	get_token_len(char *cmd, int *b_space);
t_node	*new_token(char *content, int b_space);
size_t	get_separetor(char *cmd);
size_t	is_sep(char c);
int	is_qoute(char c);
size_t	get_close_token(char *cmd, char c, int *b_space);
int     ft_strcmp(const char *s1, const char *s2);
char *strjoin_and_free(char *s1, char *s2);
t_node *create_node(const char *content, int b_space, int b_q);
t_node *create_node2(const char *content, int b_space, int type, int b_q);
void remove_node(t_node **head, t_node **end);
void join_b_space_nodes(t_node **head);
void	token_type(t_node *node);

void	add_back1(t_files **list, t_files *node);
void add_back(t_node **list, t_node *node);
void add_back2(t_env **list, t_env *node);
void	add_back3(t_herdoc **list, t_herdoc *node);
void print_list(t_node *list);


// AST TREE
//t_tree *pars_command(t_node **list);
t_tree	*parss_one_side(t_node **list);
t_tree  *parss_redirection(t_tree *node, t_node **list);
t_tree	*parss_redirection_in_start(t_node **list);
t_tree  *command_without_redirection(t_node **list);
t_tree  *pars_pipe(t_node **list);
void	collect_herdoc(t_tree *node, t_node *list);
char	*get_last_herdoc(t_herdoc *list);
char	*get_last_file(char **list);
void	print_f(int *list);

// free
void free_env(t_env *env);
void free_str(char **list);
void free_list(t_node **list);


int check_quotes(char *cmd, size_t i);
char	*remove_quotes(char *cmd);
int is_separator(char *line, int i);
int	is_empty(char *s);
int	is_space(char c);
int is_real_separator(char *cmd, int i);
int count_args(t_node *list);
int check_sides(t_node *list);
int readline_and_parssing(t_mini *minishell, t_env *env);
int     check_syntax(t_node *list);
int	is_redirection(t_node *node);
int global(int state);

// EXPANSION
void  expand(t_node **list, t_env *env);
int	is_dollar(char *cmd, int i);
char	*get_env_key(char *cmd, int i);
char	*expansion(char *cmd, t_env *list, int b_q);
int	contain_quoted(char *cmd, int len);
int	between_quoted(char *cmd, int len);
int	get_full_len(char *cmd, t_env *list);
int	split_len(char *content);
void	expand_exit_status(t_share *share);
int	get_env_len(char *cmd, int i);

// EXECUTE 
void execute_full_command(t_tree *node, t_env **env, char **envp, int pipe_flag);
// PIPE
void	execute_pipe_node(t_tree *tree, t_env *env, char **envp);
// SIMPLE
void	execute_command_node(t_tree *node, t_env **env, char **envp);
void	print(char *command, char *message, int code);
void	folder(t_env *env, char *command);
void	command_is_directory(t_env *env, char *command);
void	command_inside_directory(t_tree *node, char **envp, t_env *env);
void	empty_command(t_tree *node, t_env *env);
void  dote_command(t_tree *node, t_env *env);
void  absolute_path(t_tree *node, t_env *env, char **envp);
// REDIRECTION
void	execute_redirection_command(t_tree *node, t_env *env, char **envp);
int	check_infile_in_directory(char *files);
int	check_in_files(char *file);
int	exist_check_permession_else_create(char *file, DIR *dir);
int	check_correct_path(char *file, char *full_path);
int	in_directory(char *file);
int	is_directory(char *file);
int	just_file(t_redirection *node, char *file, int type);
int	check_out_files(t_redirection *node, char *file, int type);
int	check_if_exist(t_redirection *node);
void	ulink_files(char **files);

char	*ft_getenv(char *key, t_env *list);
char	*find_path(t_tree *node, t_env *list);
int	ft_arraylen(char **arr);
int	len_slash(char *str, char c, int len);
int only_space(char *str);

// Print data
void	print_her(t_herdoc *herdoc);
void	print_data(t_redirection *node);
void	print_redirection_data(t_tree *tree);
void	print_message(char *file, char *message);

//SIGNALS 

// int   g_ctrl_c; 
void handle_signal();
int sig_ctrl(int state);
int	ft_return_signal(int status);

// Garbage collotor
void	*ft_malloc(int type, int size);
void	ft_free_garbage(t_gcollect **list);
t_gcollect	**ft_function(void);
char *get_next_line(int fd);
#endif
