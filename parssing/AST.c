/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AST.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 11:40:58 by mbounoui          #+#    #+#             */
/*   Updated: 2025/05/26 10:13:48 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <unistd.h>

t_tree	*pars_one_side(t_node **list)
{
	t_tree	*node;
	int	i;
	char	*cmd;
	char	**args;

	i = 0;
	cmd = (*list)->content;
	args = malloc(sizeof(char *) * 100);
	args[i++] = cmd;
	*list = (*list)->next;
	while ((*list) && (*list)->type == WORD)
	{
		args[i] = (*list)->content;
		i++;
		*list = (*list)->next;
	}
	args[i] = NULL;
	node = malloc(sizeof(t_tree));
	if (!node)
		return (NULL);
	node->command = malloc(sizeof(t_commaned));
	if (!node->command)
		return (NULL);
	node->command->command = cmd;
	node->command->args = args;
	node->type = COMMAND_NODE;
	return (node);
}

t_tree	*pars_pipe(t_node **list)
{
	t_tree *left = pars_one_side(list);
	while (*list && (*list)->type == PIPE)
	{
		*list = (*list)->next;
		t_tree	*right = pars_one_side(list);
		t_tree	*pipe_cmd = malloc(sizeof(t_tree));
		if (!pipe_cmd)
			return (NULL);
		pipe_cmd->pipe = malloc(sizeof(t_pipe));
		if (!pipe_cmd->pipe)
			return (NULL);
		pipe_cmd->pipe->right = right;
		pipe_cmd->pipe->left = left;
		pipe_cmd->type = PIPE_NODE;
		left = pipe_cmd;
	}
	return left;
}

t_tree	*pars_command(t_node **list)
{
	t_tree	*node =  pars_pipe(list);
	while (*list && (*list)->type == RUN_BACKGROUND)
		*list = (*list)->next;
	return (node);
}
