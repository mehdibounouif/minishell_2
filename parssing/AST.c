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
	if (is_redirection(*list))
	{
		redirect_node = malloc(sizeof(t_tree));
		if (!redirect_node)
		{
			free_tree(&node);
			return (NULL);
		}
		redirect_node->redirect = malloc(sizeof(t_redirection));
		if (!redirect_node->redirect)
		{
			free_tree(&node);
			free(redirect_node);
			return (NULL);
		}
		redirect_node->type = REDIRECT_NODE;
		redirect_node->redirect->redirect = (*list)->content;
		*list = (*list)->next;
		while ((*list)->type == WORD && is_redirection((*list)->next))
		{
			*list = (*list)->next;
			*list = (*list)->next;
		}
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
	{
		free_str(args);
		return (NULL);
	}
	node->command = malloc(sizeof(t_command));
	if (!node->command)
	{
		free_str(args);
		free(node);
		return (NULL);
	}
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
	if (!left)
		return (NULL);
	while (*list && (*list)->type == PIPE)
	{
		*list = (*list)->next;
		t_tree	*right = pars_one_side(list);
		t_tree	*pipe_cmd = malloc(sizeof(t_tree));
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

t_tree	*pars_end(t_node **list)
{
	t_tree	*left;

	left = pars_pipe(list);
	if (!left)
		return (NULL);
	while (*list && (*list)->type == END)
	{
		if ((*list)->next && (*list)->type == END)
		{
			*list = (*list)->next;
			t_tree	*right = pars_pipe(list);
			t_tree	*end_cmd = malloc(sizeof(t_tree));
			if (!end_cmd)
			{
				free_tree(&right);
				free_tree(&left);
				return (NULL);
			}
			end_cmd->end = malloc(sizeof(t_end));
			if (!end_cmd->end)
			{
				free_tree(&right);
				free_tree(&left);
				free(end_cmd);
				return (NULL);
			}
			end_cmd->end->right = right;
			end_cmd->end->left = left;
			end_cmd->type = END_NODE;
			left = end_cmd;
		}
		else if (!(*list)->next && (*list)->type == END)
		{
			t_tree	*end_cmd = malloc(sizeof(t_tree));
			if (!end_cmd)
			{
				free_tree(&left);
				return (NULL);
			}
			end_cmd->end = malloc(sizeof(t_end));
			if (!end_cmd->end)
			{
				free_tree(&left);
				free(end_cmd);
				return (NULL);
			}
			end_cmd->end->left = left;
			end_cmd->end->right = NULL;
			end_cmd->type = END_NODE;
			left = end_cmd;
			break;
		}
	}
	return (left);
}

t_tree	*pars_command(t_node **list)
{
	t_tree	*tree;
	
	tree =  pars_end(list);
	if (!tree)
	{
		free_list(list);
		return (NULL);
	}
	return (tree);
}
