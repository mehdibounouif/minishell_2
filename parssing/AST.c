/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AST.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 11:40:58 by mbounoui          #+#    #+#             */
/*   Updated: 2025/05/30 09:42:21 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_tree	*parss_redirection(t_tree *node, t_node **list)
{
	t_tree	*redirect_node;

	redirect_node = NULL;
	if ((*list) && ((*list)->type == REDIRECTION_OUT ||
			(*list)->type == REDIRECTION_IN ||
			(*list)->type == REDIRECTION_ERR ||
			(*list)->type == REDIRECTION_APPEND))
	{
		redirect_node = malloc(sizeof(t_tree));
		if (!redirect_node)
			return (NULL);// free
		redirect_node->redirect = malloc(sizeof(t_redirection));
		if (!redirect_node->redirect)
			return (NULL);
		redirect_node->type = REDIRECT_NODE;
		redirect_node->redirect->redirect = (*list)->content;
		*list = (*list)->next;
		redirect_node->redirect->file = (*list)->content;
		redirect_node->redirect->redirection_type = (*list)->type;
		redirect_node->redirect->prev = node;
		*list = (*list)->next;
	}
	return (redirect_node);
}

t_tree	*pars_one_side(t_node **list)
{
	t_tree	*node;
	t_tree	*redirct_node;
	int	i;
	char	*cmd;
	char	**args;
  	int len;

	i = 0;
	len = count_args(*list);
	cmd = (*list)->content;
	args = malloc(sizeof(char *) * len + 1);
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
	node->command = malloc(sizeof(t_command));
	if (!node->command)
		return (NULL);
	node->command->command = cmd;
	node->command->args = args;
	node->type = COMMAND_NODE;
	redirct_node = parss_redirection(node, list);
	if (redirct_node)
		node = redirct_node;
	return (node);
}

t_tree	*pars_pipe(t_node **list)
{
	t_tree *left;

       	left = pars_one_side(list);
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

t_tree	*pars_end(t_node **list)
{
	t_tree	*left;

	left = pars_pipe(list);
	while (*list && (*list)->type == END)
	{
		*list = (*list)->next;
		t_tree	*right = pars_pipe(list);
		t_tree	*end_cmd = malloc(sizeof(t_tree));
		if (!end_cmd)
			return (NULL);
		end_cmd->end = malloc(sizeof(t_end));
		if (!end_cmd->end)
			return (NULL);
		end_cmd->end->right = right;
		end_cmd->end->left = left;
		end_cmd->type = END_NODE;
		left = end_cmd;
	}
	return (left);
}

/*

t_tree	*pars_operator(t_node **list)
{
	t_tree *left = pars_pipe(list);
	int	i;

	while ((*list) && ((*list)->type == AND || (*list)->type == OR))
	{
		i = (*list)->type;
		*list = (*list)->next;
		t_tree	*right = pars_pipe(list);
		t_tree	*cmd = malloc(sizeof(t_tree));
		if (!cmd)
			return (NULL);
		if (i == OR)
		{
			cmd->orr = malloc(sizeof(t_or));
			if (!cmd->orr)
				return (NULL);
			cmd->type = OR_NODE;
			cmd->orr->left = left;
			cmd->orr->right = right;
		}
		else if (i == AND)
		{
			cmd->andd = malloc(sizeof(t_and));
			if (!cmd->andd)
				return (NULL);
			cmd->type = AND_NODE;
			cmd->andd->left = left;
			cmd->andd->right = right;
		}
		left = cmd;
	}
	return (left);
}

t_tree	*pars_or(t_node **list)
{
	t_tree	*left = pars_and(list);
	while ((*list) && (*list)->type == OR)
	{
		*list = (*list)->next;
		t_tree	*right = pars_and(list);
		t_tree	*or_cmd = malloc(sizeof(t_tree));
		if (!or_cmd)
			return (NULL);
		or_cmd->orr = malloc(sizeof(t_or));
		if (!or_cmd->orr)
			return (NULL);
		or_cmd->type = OR_NODE;
		or_cmd->orr->left = left;
		or_cmd->orr->right = right;
		left = or_cmd;
	}
	return (left);
}

t_tree	*parss_op(t_node **list)
{
	if ((*list)->type == )
}
*/

t_tree	*pars_command(t_node **list)
{
	t_tree	*tree;
	
	tree =  pars_end(list);
	if (!tree)
		return (NULL);
	//while (*list && (*list)->type == RUN_BACKGROUND)
	//	*list = (*list)->next;
	return (tree);
}
