/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_ast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 09:02:11 by mbounoui          #+#    #+#             */
/*   Updated: 2025/07/14 10:52:13 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

 #include "../../includes/minishell.h"

// #define COLOR_RESET   "\033[0m"
// #define COLOR_CMD     "\033[1;32m"
// #define COLOR_PIPE    "\033[1;34m"
// #define COLOR_RED     "\033[1;33m"
// #define COLOR_AND     "\033[1;36m"
// #define COLOR_OR      "\033[1;35m"

// char *build_prefix(const char *prefix, int is_last) {
//     const char *suffix = is_last ? "    " : "│   ";
//     size_t len = strlen(prefix) + strlen(suffix) + 1;
//     char *result = malloc(len);
//     if (!result) {
//         perror("malloc");
//         exit(EXIT_FAILURE);
//     }
//     strcpy(result, prefix);
//     strcat(result, suffix);
//     return result;
// }

// void print_tree_unicode(t_tree *tree, const char *prefix, int is_last) {
//     if (!tree) return;

//     printf("%s%s", prefix, is_last ? "└── " : "├── ");

//     switch (tree->type) {
//         case COMMAND_NODE:
//             printf(COLOR_CMD "COMMAND: %s [" COLOR_RESET, tree->command->command);
//             for (int i = 0; tree->command->args[i]; i++) {
//                 printf("%s,", tree->command->args[i]);
//             }
//             printf(COLOR_CMD "]\n" COLOR_RESET);
//             break;

//         case PIPE_NODE: {
//             printf(COLOR_PIPE "PIPE\n" COLOR_RESET);
//             char *new_prefix = build_prefix(prefix, is_last);
//             print_tree_unicode(tree->pipe->left, new_prefix, 0);
//             print_tree_unicode(tree->pipe->right, new_prefix, 1);
//             free(new_prefix);
//             break;
//         }
//         case REDIRECT_NODE: {
//             printf(COLOR_RED "REDIRECT: Input file => %s\n" COLOR_RESET,
//                     tree->redirect->in_file);
// 			printf("%s%s", prefix, is_last ? "└── " : "├── ");
//             printf(COLOR_RED "REDIRECT: Output file => %s\n" COLOR_RESET,
//                    tree->redirect->out_file);
//             char *new_prefix = build_prefix(prefix, is_last);
//             print_tree_unicode(tree->redirect->prev, new_prefix, 1);
//             free(new_prefix);
//             break;
//         }
//     }
// }

#define COLOR_RESET   "\033[0m"
#define COLOR_CMD     "\033[1;32m"
#define COLOR_ARG     "\033[0;36m"
#define COLOR_RED     "\033[1;31m"
#define COLOR_PIPE    "\033[1;35m"
#define COLOR_INFO    "\033[0;33m"
#define TREE_BRANCH   "├── "
#define TREE_CHILD    "│   "
#define TREE_END      "└── "
#define TREE_SPACE    "    "


void print_command_node(t_command *cmd, int level) {
    print_indent(level, 0);
    printf(COLOR_CMD "COMMAND: %s\n" COLOR_RESET, cmd->command);
    
    if (cmd->args) {
        print_indent(level + 1, 1);
        printf(COLOR_ARG "ARGS: ");
        for (int i = 0; cmd->args[i]; i++)
            printf("[%s] ", cmd->args[i]);
        printf(COLOR_RESET "\n");
    }
}

void print_heredocs(t_herdoc *h, int level) {
    while (h) {
        print_indent(level, 0);
        printf(COLOR_RED "HEREDOC: '%s' (quoted=%d)\n" COLOR_RESET, h->delimeter, h->quoted);
        h = h->next;
    }
}

void print_redirect_node(t_redirection *r, int level) {
    print_indent(level, 0);
    printf(COLOR_RED "REDIRECT NODE\n" COLOR_RESET);

    if (r->in_file) {
        print_indent(level + 1, 0);
        printf(COLOR_INFO "IN: %s\n" COLOR_RESET, r->in_file);
    }
    if (r->out_file) {
        print_indent(level + 1, 0);
        printf(COLOR_INFO "OUT: %s\n" COLOR_RESET, r->out_file);
    }

    if (r->herdoc)
        print_heredocs(r->herdoc, level + 1);

    if (r->prev) {
        print_indent(level + 1, 0);
        printf("Redirected Command:\n");
        print_ast(r->prev, level + 2);
    }
}

void print_pipe_node(t_pipe *p, int level) {
    print_indent(level, 0);
    printf(COLOR_PIPE "PIPE NODE\n" COLOR_RESET);

    print_indent(level + 1, 0);
    printf("Left:\n");
    print_ast(p->left, level + 2);

    print_indent(level + 1, 0);
    printf("Right:\n");
    print_ast(p->right, level + 2);
}


void print_ast(t_tree *tree, int level)
{
    if (!tree)
        return;
    if (tree->type == COMMAND_NODE)
        print_command_node(tree->command, level);
    else if (tree->type == REDIRECT_NODE)
        print_redirect_node(tree->redirect, level);
    else if (tree->type == PIPE_NODE)
        print_pipe_node(tree->pipe, level);
}


void  print_env(t_env *env)
{
	while (env && env->next)
	{
		printf("key = %s\n", env->key);
		printf("value = %s\n", env->value);
		env = env->next;
	}
}

void	print_her(t_herdoc *herdoc)
{
    if (!herdoc) return;
	printf("{");
	while (herdoc)
	{
		printf("%s ", herdoc->herdoc);
		printf("%s ,", herdoc->delimeter);
		herdoc = herdoc->next; }
	printf("} ->\n");
}

void	print_data(t_redirection *node)
{
	int	i;

	i = 0;
	printf("Input file = %s\n", node->in_file);
	printf("Output file = %s\n", node->out_file);
	printf("Input flag = %d\n", node->in);
	printf("Herdoc flag = %d\n", node->hdc);
//	printf("Herdoc fd = %d\n", node->fd);
	printf("\nInput files :\n");
	while (node->in_files[i])
	{
		printf("file %d = %s\n", i+1, node->in_files[i]);
		i++;
	}
	printf("\nOutput files :\n");
	i = 0;
	while (node->out_files[i])
	{
		printf("file %d = %s\n", i+1, node->out_files[i]);
		i++;
	}
	printf("\nHerdoc fds :\n");
	i = 0;
	while (node->heredoc_fds[i])
	{
		printf("fd %d = %d\n", i+1, node->heredoc_fds[i]);
		i++;
	}
	printf("\nHere documents :\n");
	print_her(node->herdoc);
	printf("\n======================\n");
}
/*
void	print_herdoc(t_tree *tree)
{
    if (!tree)
		return;

	switch (tree->type) {
		case REDIRECT_NODE:
		print_her(tree->redirect->herdoc);
		break;

		case PIPE_NODE:
		print_herdoc(tree->pipe->left);
		print_herdoc(tree->pipe->right);
		break;
	}
	printf("\n");
}
*/
void	print_redirection_data(t_tree *tree)
{
	if (tree->type == REDIRECT_NODE)
	{
		print_data(tree->redirect);
	}
	else if (tree->type == PIPE_NODE)
	{
		print_redirection_data(tree->pipe->left);
		print_redirection_data(tree->pipe->right);
	}
	else
		return ;
}












