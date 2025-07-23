/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_tree.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 10:28:42 by mbounoui          #+#    #+#             */
/*   Updated: 2025/07/23 11:16:17 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_command_node1(t_tree *tree)
{
	if (tree)
	{
		if (tree->command->args)
			free_str(tree->command->args);
		if (tree->command)
			free(tree->command);
		free(tree);
	}
}

void	free_tree(t_tree **tree)
{
	if (!tree)
		return ;
	if ((*tree)->type == COMMAND_NODE)
	{
		free_command_node1(*tree);
	}
	else if ((*tree)->type == PIPE_NODE)
	{
		free_tree(&(*tree)->pipe->left);
		free_tree(&(*tree)->pipe->right);
		free((*tree)->pipe);
		(*tree)->pipe = NULL;
	}
	else if ((*tree)->type == REDIRECT_NODE)
	{
		free_tree(&(*tree)->redirect->prev);
		free_redirect_node((*tree)->redirect);
	}
//	free(*tree);
//	*tree = NULL; 
}

void	ft_free(t_mini *minishell)
{
	free_list(&minishell->list);
	free_tree(&minishell->tree);
}
