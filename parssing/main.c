/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 08:03:44 by mbounoui          #+#    #+#             */
/*   Updated: 2025/08/12 17:32:27 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	global(int state)
{
	static int	value;

	if (state != -1)
		value = state;
	return (value);
}


// #define COLOR_RESET   "\033[0m"
// #define COLOR_CMD     "\033[1;32m"
// #define COLOR_ARG     "\033[0;36m"
// #define COLOR_RED     "\033[1;31m"
// #define COLOR_PIPE    "\033[1;35m"
// #define COLOR_INFO    "\033[0;33m"
// #define TREE_BRANCH   "├── "
// #define TREE_CHILD    "│   "
// #define TREE_END      "└── "
// #define TREE_SPACE    "    "
// #include <stdio.h>

// void print_indent(int level, int is_arg) {
//     // If it's for arguments or something else, use different indenting style
//     for (int i = 0; i < level; i++) {
//         if (is_arg)
//             printf("    "); // more indent for argument nodes
//         else
//             printf(TREE_BRANCH);  // standard indentation (could be adjusted as per needs)
//     }
// }


// void print_command_node(t_command *cmd, int level) {
//     print_indent(level, 0);
//     printf(COLOR_CMD "COMMAND: %s\n" COLOR_RESET, cmd->command);
    
//     if (cmd->args) {
//         print_indent(level + 1, 1);
//         printf(COLOR_ARG "ARGS: ");
//         for (int i = 0; cmd->args[i]; i++)
//             printf("[%s] ", cmd->args[i]);
//         printf(COLOR_RESET "\n");
//     }
// }

// void print_heredocs(t_herdoc *h, int level) {
//     while (h) {
//         print_indent(level, 0);
//         printf(COLOR_RED "HEREDOC: '%s' (quoted=%d)\n" COLOR_RESET, h->delimeter, h->quoted);
//         h = h->next;
//     }
// }

// void print_redirect_node(t_redirection *r, int level) {
//     print_indent(level, 0);
//     printf(COLOR_RED "REDIRECT NODE\n" COLOR_RESET);

//     if (r->in_file) {
//         print_indent(level + 1, 0);
//         printf(COLOR_INFO "IN: %s\n" COLOR_RESET, r->in_file);
//     }
//     if (r->out_file) {
//         print_indent(level + 1, 0);
//         if (r->out_type == R_APPEND)
//             printf("APPEND:");
//         else
//           printf("OUT:");
//         printf(COLOR_INFO "%s\n" COLOR_RESET, r->out_file);
//     }

//     if (r->herdoc)
//         print_heredocs(r->herdoc, level + 1);

//     if (r->prev) {
//         print_indent(level + 1, 0);
//         printf("Redirected Command:\n");
//         print_ast(r->prev, level + 2);
//     }
// }

// void print_pipe_node(t_pipe *p, int level) {
//     print_indent(level, 0);
//     printf(COLOR_PIPE "PIPE NODE\n" COLOR_RESET);

//     print_indent(level + 1, 0);
//     printf("Left:\n");
//     print_ast(p->left, level + 2);

//     print_indent(level + 1, 0);
//     printf("Right:\n");
//     print_ast(p->right, level + 2);
// }


// void print_ast(t_tree *tree, int level)
// {
//     if (!tree)
//         return;
//     if (tree->type == COMMAND_NODE)
//         print_command_node(tree->command, level);
//     else if (tree->type == REDIRECT_NODE)
//         print_redirect_node(tree->redirect, level);
//     else if (tree->type == PIPE_NODE)
//         print_pipe_node(tree->pipe, level);
// }

void	mini_shell(t_mini *minishell, char **env, t_env *envp)
{
	while (1)
	{
		handle_signal();
		if (!readline_and_parssing(minishell, envp))
			continue ;
    //print_ast(minishell->tree, 0);
		if (!open_herdocs(minishell->tree, envp))
			continue ;
    sig_ctrl(1);
    execute_full_command(minishell->tree, &envp, env, 0);
    sig_ctrl(0);
	}
}

static void	init_pwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		perror("minishell ::");
	else
		*(back_up()) = ft_strdup(cwd);
	free(cwd);
}

int	main(int c, char **v __attribute__((unused)), char **env)
{
	t_mini	minishell;
	t_env	*envp;

	if (c != 1)
	{
		ft_putendl_fd("This program does not accept arguments", 2);
		exit(0);
	}
	init_pwd();
	envp = NULL;
	ft_bzero(&minishell, sizeof(t_mini));
	if (!get_env(&envp, env))
		exit(0);
	global(0);
	mini_shell(&minishell, env, envp);
	ft_free_garbage(ft_function());
	exit(global(-1));
}
