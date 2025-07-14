/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_with_pipe.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 16:38:25 by mbounoui          #+#    #+#             */
/*   Updated: 2025/07/14 11:59:47 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_tree  *pars_pipe(t_node **list, t_env *env)
{
	t_tree *left;

	left = parss_one_side(list, env);
	if (!left)
		return (NULL);
	while (*list && (*list)->type == PIPE)
	{
		*list = (*list)->next;
		t_tree  *right = parss_one_side(list, env);
		t_tree  *pipe_cmd = malloc(sizeof(t_tree));
		if (!pipe_cmd)
		{
			free_tree(&right);
			free_tree(&left);
			return (NULL);
		}
		pipe_cmd->pipe = malloc(sizeof(t_pipe));
		if (!pipe_cmd->pipe)
		{
			free_tree(&right);
			free_tree(&left);
			free(pipe_cmd);
			return (NULL);
		}
		pipe_cmd->pipe->right = right;
		pipe_cmd->pipe->left = left;
		pipe_cmd->type = PIPE_NODE;
		left = pipe_cmd;
	}
	return left;
}
