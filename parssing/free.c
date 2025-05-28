/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 08:03:10 by mbounoui          #+#    #+#             */
/*   Updated: 2025/05/28 09:12:01 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_str(char **list)
{
	char *tmp;
	while (*list)
	{
		tmp = *list;
		list++;
		free(tmp);
	}
}

void	free_list(t_node **list)
{
	t_node *tmp;

	tmp = *list;
	while (*list)
	{
		*list = (*list)->next;
		if ((*list)->content)
			free(tmp->content);
		free(tmp);
		tmp = *list;
	}
}

void	free_tree(t_tree **tree)
{
	if ((*tree)->type == COMMAND_NODE)
	{
		free_str((*tree)->command->args);
		free((*tree)->command);
		//free((*tree));
	}
	else if ((*tree)->type == PIPE_NODE)
	{
		free_tree(&(*tree)->pipe->left);
		free_tree(&(*tree)->pipe->right);
		free((*tree)->pipe);
	}
	else if ((*tree)->type == REDIRECT_NODE)
	{
		free_tree(&(*tree)->redirect->prev);
		free((*tree)->redirect);
	}
}

void	ft_free(t_node **list, t_tree **tree)
{
	free_tree(tree);
	free_list(list);
}
