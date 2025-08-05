/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   designing_tree.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 16:38:25 by mbounoui          #+#    #+#             */
/*   Updated: 2025/07/30 13:20:45 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
	return left;
}

t_tree  *pars_pipe(t_node **list)
{
	t_tree *left;

	left = parss_one_side(list);
	while (*list && (*list)->type == PIPE)
	{
		*list = (*list)->next;
		t_tree  *right = parss_one_side(list);
		t_tree  *pipe_cmd = ft_malloc(sizeof(t_tree), 1);
		pipe_cmd->pipe = ft_malloc(sizeof(t_pipe), 1);
		pipe_cmd->pipe->right = right;
		pipe_cmd->pipe->left = left;
		pipe_cmd->type = PIPE_NODE;
		left = pipe_cmd;
	}
	return left;
}
