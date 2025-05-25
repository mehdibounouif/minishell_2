/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AST.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 11:40:58 by mbounoui          #+#    #+#             */
/*   Updated: 2025/05/25 16:27:11 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <unistd.h>

t_tree	*pars_one_side(t_node **list)
{
	return (NULL);
}

t_tree	*pars_pipe(t_node **list)
{
	t_tree *left = pars_one_side(list);
	while ((*list)->type == PIPE)
	{
		*list = (*list)->next;
		t_tree	*right = pars_one_side(list);
		t_tree	*pipe_cmd = malloc(sizeof(t_tree));
		pipe_cmd->pipe->right = right;
		pipe_cmd->pipe->left = left;
		pipe_cmd->type = REDIRECT_NODE;
		left = pipe_cmd;
		*list = (*list)->next;
	}
	return left;
}

t_tree	*pars_command(t_node **list)
{
	t_tree	*node =  pars_pipe(list);
	while ((*list)->type == RUN_BACKGROUND)
	{
		*list = (*list)->next;
	}
	return (node);
}
