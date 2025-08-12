/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   designing_tree.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moraouf <moraouf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 16:38:25 by mbounoui          #+#    #+#             */
/*   Updated: 2025/08/12 13:13:38 by moraouf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	init(t_tree *node)
{
	node->type = REDIRECT_NODE;
	node->redirect->in = 0;
	node->redirect->out_type = 0;
	node->redirect->in_count = 0;
	node->redirect->out_count = 0;
	node->redirect->hdc = 0;
	node->redirect->files = NULL;
	node->redirect->herdoc = NULL;
	node->redirect->heredocs = NULL;
}

t_tree	*parss_one_side(t_node **list)
{
	t_tree	*left;

	if (is_redirection(*list))
	{
		left = parss_redirection_in_start(list);
	}
	else
	{
		left = command_without_redirection(list);
		if (is_redirection((*list)))
			left = parss_redirection(left, list);
	}
	return (left);
}

static t_tree	*create_pipe_node(t_tree *left, t_tree *right)
{
	t_tree	*pipe_cmd;

	pipe_cmd = ft_malloc(sizeof(t_tree), 1);
	pipe_cmd->pipe = ft_malloc(sizeof(t_pipe), 1);
	pipe_cmd->pipe->right = right;
	pipe_cmd->pipe->left = left;
	pipe_cmd->type = PIPE_NODE;
	return (pipe_cmd);
}

static t_tree	*process_pipe_side(t_node **list)
{
	t_tree	*side;

	if ((*list)->type == PIPE)
		side = NULL;
	else
		side = parss_one_side(list);
	return (side);
}

t_tree	*pars_pipe(t_node **list)
{
	t_tree	*left;
	t_tree	*right;

	left = process_pipe_side(list);
	while (*list && (*list)->type == PIPE)
	{
		*list = (*list)->next;
		if (*list)
			right = process_pipe_side(list);
		else
			right = NULL;
		left = create_pipe_node(left, right);
	}
	return (left);
}
