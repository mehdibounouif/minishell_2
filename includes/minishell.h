/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moraouf <moraouf@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 08:10:15 by mbounoui          #+#    #+#             */
/*   Updated: 2025/08/11 23:14:03 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <dirent.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <limits.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>
# include <errno.h>

# define EMPTY 0
# define PIPE 1
# define WORD 2
# define R_OUT 3
# define R_IN 4
# define R_APPEND 5
# define HEREDOC 9

// TYPES OF NODES

# define COMMAND_NODE 10
# define PIPE_NODE 11
# define REDIRECT_NODE 12
# define END_NODE 13

# define HEREDOC_FILE ".heredoc_file_"

typedef struct s_command		t_command;
typedef struct s_redirection	t_redirection;
typedef struct s_tree			t_tree;
typedef struct s_node			t_node;
typedef struct s_pipe			t_pipe;
typedef struct s_end			t_end;
typedef struct s_env			t_env;
typedef struct s_mini			t_mini;
typedef struct s_herdoc			t_herdoc;
typedef struct s_files			t_files;

struct							s_env
{
	char						*key;
	char						*value;
	struct s_env				*next;
};

struct							s_node
{
	int							contain_quoted;
	int							between_quoted;
	int							type;
	int							b_space;
	char						*content;
	struct s_node				*next;
	struct s_node				*prev;
};

struct							s_pipe
{
	t_tree						*left;
	t_tree						*right;
};

struct							s_command
{
	char						*command;
	char						**args;
};

struct							s_herdoc
{
	char						*file;
	char						*herdoc;
	char						*delimeter;
	int							quoted;
	struct s_herdoc				*next;
};

struct							s_files
{
	int							type;
	char						*file;
	struct s_files				*next;
};

struct							s_redirection
{
	t_tree						*prev;
	char						*in_file;
	char						**in_files;
	char						*out_file;
	char						**out_files;
	char						**heredocs;
	char						*hrc_file;
	int							in;
	int							hdc;
	int							in_count;
	int							out_count;
	int							out_type;
	t_files						*files;
	t_herdoc					*herdoc;
};

struct							s_tree
{
	int							type;
	int							ret;
	t_command					*command;
	t_pipe						*pipe;
	t_redirection				*redirect;
};

struct							s_mini
{
	t_tree						*tree;
	t_node						*list;
};

typedef struct s_gcollect
{
	void						*value;
	struct s_gcollect			*next;
}								t_gcollect;

// expantion struct
typedef struct t_share
{
	int							i;
	int							j;
	int							h;
	char						*expanded_cmd;
}								t_share;


typedef struct s_share4
{
    t_tree    *redirect_node;
    t_tree    *prev;
    t_node    *tmp;
    int        i;
    int        j;

}            t_share4;
// built-in functions
// Function declarations
int								cd_command(t_env *env, char **args);
int								echo_command(char **args);
int								pwd_command(void);
int								export_command(t_env **env, char **args);
int								unset_command(t_env **env, char **args);
int								env_command(t_env *env, char **args);
int								exit_command(int i, t_tree *node, t_env *env,
									char **args);
// exit_utils.c helper functions
int								handle_sign(char *str, int i, int *sign);
int								validate_digits(char *str, int start);
int								check_overflow(long long result, char digit,
									int negative);
long long						parse_number(char *str, int *i, int sign,
									int *error);
int								normalize_exit_status(long long status);
char							*get_env_value(t_env *env, char *key);
int								update_env_var(t_env *env, char *var);
void							set_env_var(t_env **env, char *key,
									char *value);
void							free_env(t_env *env);
void							add_env_var(t_env **env, char *key,
									char *value);
void							sorted_env(t_env *env);
void							ft_lstadd_node(t_env **lst, t_env *node);
t_env							*create_env_var(char *key, char *value);
t_env							*get_env_var(t_env *env, const char *key);
int								get_env(t_env **envp, char **env);
void							print_env_export(t_env *env);

// Custom_libft
char							*ft_strjoin1(char const *s1, char const *s2);
char							*ft_substr1(char const *s, unsigned int start,
									size_t len);
char							*ft_strdup1(const char *s);
char							**ft_split1(char const *s, char c);

// Command execution
// PIPE
void							execute_pipe_node(t_tree *tree, t_env *env,
									char **envp);
void							print_env(t_env *env);
void							print_ast(t_tree *tree, int level);
// int	open_herdocs(t_tree *tree, t_env *env);
void							skip_redirection(t_node **list);
char							**back_up(void);
void							print(char *command, char *message, int code);
int								execute_builtin(int i, t_tree *node, t_env **env);
int								is_builtin(char *command);
int								execute_builtin_command(int i, char *command,
									char **args, t_env **env);
void							dup_fds(t_redirection *node, t_env *env);

// Tokenize
void							tokenize(char *cmd, t_node **list);
size_t							get_token_len(char *cmd, int *b_space);
t_node							*new_token(char *content, int b_space);
size_t							get_separetor(char *cmd);
size_t							is_sep(char c);
int								is_qoute(char c);
size_t							get_close_token(char *cmd, char c,
									int *b_space);
int								ft_strcmp(const char *s1, const char *s2);
t_node							*create_node(const char *content, int b_space,
									int b_q);
t_node							*create_node2(const char *content, int b_space,
									int type, int b_q);
void							remove_node(t_node **head, t_node **end);
void							join_b_space_nodes(t_node **head);
void							token_type(t_node *node);

void							add_back1(t_files **list, t_files *node);
void							add_back(t_node **list, t_node *node);
void							add_back2(t_env **list, t_env *node);
void							add_back3(t_herdoc **list, t_herdoc *node);
void							print_list(t_node *list);

// AST TREE
t_tree							*parss_one_side(t_node **list);
t_tree							*parss_redirection(t_tree *node, t_node **list);
t_tree							*parss_redirection_in_start(t_node **list);
t_tree							*command_without_redirection(t_node **list);
t_tree							*pars_pipe(t_node **list);
void							collect_herdoc(t_tree *node, t_node *list);
char							*get_last_herdoc(t_herdoc *list);
void							print_f(int *list);

// free
void							free_env(t_env *env);
void							free_str(char **list);
void							free_list(t_node **list);

int								check_quotes(char *cmd, size_t i);
char							*remove_quotes(char *cmd);
int								is_separator(char *line, int i);
int								is_empty(char *s);
int								is_space(char c);
int								is_real_separator(char *cmd, int i);
int								count_args(t_node *list);
int								readline_and_parssing(t_mini *minishell,
									t_env *env);
int								check_syntax(t_node *list, int flag);
int								is_redirection(t_node *node);
int								global(int state);

// EXPANSION
void							expand(t_node **list, t_env *env);
int								is_dollar(char *cmd, int i);
char							*get_env_key(char *cmd, int i);
char							*expansion(char *cmd, t_env *list, int b_q);
int								contain_quoted(char *cmd, int len);
int								between_quoted(char *cmd, int len);
int								get_full_len(char *cmd, t_env *list);
void							expand_exit_status(t_share *share);
int								get_env_len(char *cmd, int i);
void							free_node(t_node *node);
void							replace_key(char *cmd, t_share *share,
									t_env *list);
t_node							*insert_sublist(t_node *start, t_node *_new,
									t_node *next);

// HEREDOC
typedef struct s_share3
{
	int							fd;
	char						*line;
	char						*file_name;
	char						*expand_line;
}								t_share3;
int								open_herdocs(t_tree *tree, t_env *env);
void							read_lines(int *flag, t_share3 *share,
									t_redirection *list, t_env *env);
char							*get_last_file(char **list);
char							*generate_file_name(t_env *env);
void							heredoc_sigint_handler(int sig
									__attribute__((unused)));
t_env							*get_current_env(t_env *env);
t_share3						*get_current_share(t_share3 *share);
void							check_line(t_share3 *share);
char	*get_last_herdoc(t_herdoc *list);
void	take_heredoc(t_tree *node, t_node *list);
void	collect_herdoc(t_tree *node, t_node *list);
void	init(t_tree *node);
void	collect_in_out_files2(t_node **list, t_share4 *share);
void	collect_in_out_files(t_node **list, t_share4 *share);
void	assing_io(t_node **list, t_share4 *share);
void	assign_last_file(t_tree *node);

// EXECUTE
void							execute_full_command(t_tree *node, t_env **env,
									char **envp, int pipe_flag);
// PIPE
void							execute_pipe_node(t_tree *tree, t_env *env,
									char **envp);
// SIMPLE
void							execute_command_node(int i, t_tree *node, t_env **env,
									char **envp);
void							print(char *command, char *message, int code);
void							empty_command(t_tree *node, t_env *env);
void							dote_command(t_tree *node, t_env *env);
void							absolute_path(t_tree *node, t_env *env,
									char **envp);
void							print_and_exit(t_tree *node, t_env *env,
									int code, char *message);
void							protect(t_env *env, char *message);
// REDIRECTION
void	execute_redirection_command(int i, t_tree *node, t_env *env, char **envp);
int								check_in_files(char *file);
int								in_directory(char *file);
int								check_if_exist(t_redirection *node);
void							ulink_files(char **files);

char							*ft_getenv(char *key, t_env *list);
char							*find_path(t_tree *node, t_env *list, int *flag);
int								ft_arraylen(char **arr);
int								len_slash(char *str, char c, int len);
int								only_space(char *str);

// Print data
void							print_her(t_herdoc *herdoc);
void							print_data(t_redirection *node);
void							print_redirection_data(t_tree *tree);
void							print_message(char *file, char *message);

// SIGNALS

// int   g_ctrl_c;
void							handle_signal(void);
int								sig_ctrl(int state);
int								ft_return_signal(int status);

// Garbage collotor
void							*ft_malloc(int type, int size);
void							ft_free_garbage(t_gcollect **list);
t_gcollect						**ft_function(void);
char							*get_next_line(int fd);
#endif
