/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_ast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 09:02:11 by mbounoui          #+#    #+#             */
/*   Updated: 2025/05/30 09:59:31 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

#define COLOR_RESET   "\033[0m"
#define COLOR_CMD     "\033[1;32m"
#define COLOR_PIPE    "\033[1;34m"
#define COLOR_RED     "\033[1;33m"
#define COLOR_AND     "\033[1;36m"
#define COLOR_OR      "\033[1;35m"

// Assume node types are defined like this:
//typedef enum { COMMAND_NODE, PIPE_NODE, REDIRECT_NODE, OR_NODE, AND_NODE } NodeType;


// Helper function to safely build the prefix string
char *build_prefix(const char *prefix, int is_last) {
    const char *suffix = is_last ? "    " : "│   ";
    size_t len = strlen(prefix) + strlen(suffix) + 1;
    char *result = malloc(len);
    if (!result) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    strcpy(result, prefix);
    strcat(result, suffix);
    return result;
}

// Your updated print_tree_unicode function
void print_tree_unicode(t_tree *tree, const char *prefix, int is_last) {
    if (!tree) return;

    printf("%s%s", prefix, is_last ? "└── " : "├── ");

    switch (tree->type) {
        case COMMAND_NODE:
            printf(COLOR_CMD "COMMAND: %s [" COLOR_RESET, tree->command->command);
            for (int i = 0; tree->command->args[i]; i++) {
                printf("%s,", tree->command->args[i]);
            }
            printf(COLOR_CMD "]\n" COLOR_RESET);
            break;

        case PIPE_NODE: {
            printf(COLOR_PIPE "PIPE\n" COLOR_RESET);
            char *new_prefix = build_prefix(prefix, is_last);
            print_tree_unicode(tree->pipe->left, new_prefix, 0);
            print_tree_unicode(tree->pipe->right, new_prefix, 1);
            free(new_prefix);
            break;
        }

        case END_NODE: {
            printf(COLOR_PIPE "END\n" COLOR_RESET);
            char *new_prefix = build_prefix(prefix, is_last);
            print_tree_unicode(tree->end->left, new_prefix, 0);
            print_tree_unicode(tree->end->right, new_prefix, 1);
            free(new_prefix);
            break;
        }

        case REDIRECT_NODE: {
            printf(COLOR_RED "REDIRECT: %s -> %s\n" COLOR_RESET,
                   tree->redirect->redirect, tree->redirect->file);
            char *new_prefix = build_prefix(prefix, is_last);
            print_tree_unicode(tree->redirect->prev, new_prefix, 1);
            free(new_prefix);
            break;
        }
    }
}

