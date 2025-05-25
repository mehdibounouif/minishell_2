/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 07:55:09 by mbounoui          #+#    #+#             */
/*   Updated: 2025/05/25 16:26:27 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include "../libft/libft.h"

# define PIPE 1
# define WORD 2
# define REDIRECTION_OUT 3
# define REDIRECTION_IN 4
# define REDIRECTION_APPEND 5
# define REDIRECTION_ERR 6
# define RUN_BACKGROUND 7
# define HEREDOC 8

// TYPES OF NODES

# define COMMAND_NODE 1
# define PIPE_NODE 2
# define REDIRECT_NODE 3

typedef	struct	s_command t_commaned;
typedef	struct	s_tree t_tree;

typedef	struct s_node
{
	int	type;
	char	*content;
	struct	s_node *next;
	struct	s_node *prev;
}	t_node;

typedef	struct	s_pipe
{
	t_tree *left;
	t_tree *right;
}	t_pipe;

typedef	struct	s_command
{
	char	*command;
	char	**args;
}	t_commaned;

typedef	struct	t_redirection
{
	t_pipe	*prev;
	char	*file;
	int	redirection_type;
}	t_redirection;

typedef	struct	s_tree
{
	int	type;
	t_commaned *command;
	t_pipe	*pipe;
	t_redirection *redirect;
} t_tree;

void	tokenize(char *command, t_node **list);
int	is_space(char c);
void	add_back(t_node **list, t_node *node);
void	print_list(t_node *list);
t_tree	*pars_command(t_node **list);
