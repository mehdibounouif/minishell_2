/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 08:03:10 by mbounoui          #+#    #+#             */
/*   Updated: 2025/05/30 09:43:13 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_str(char **list)
{
  int i;

  if (!list || !*list)
    return ;
  i = 0;
	while (i < 5)
	{
    free(list[i]);
    i++;
	}
	free(list);
}

void	free_list(t_node **list)
{
	t_node *tmp;

  if (!list || !*list)
    return ;
	tmp = *list;
	while (*list)
	{
		*list = (*list)->next;
    if (tmp->content)
		  free(tmp->content);
		free(tmp);
		tmp = *list;
	}
  *list = NULL;
}

void	free_tree(t_tree **tree)
{
  if (!tree || !*tree)
    return ;
	if ((*tree)->type == COMMAND_NODE)
	{
		free((*tree)->command->args);
		free((*tree)->command);
	}
	else if ((*tree)->type == PIPE_NODE)
	{
		free_tree(&(*tree)->pipe->left);
		free_tree(&(*tree)->pipe->right);
		free((*tree)->pipe);
		(*tree)->pipe = NULL;
	}
	else if ((*tree)->type == END_NODE)
	{
		free_tree(&(*tree)->end->left);
		free_tree(&(*tree)->end->right);
		free((*tree)->end);
		(*tree)->end = NULL;
	}
	else if ((*tree)->type == REDIRECT_NODE)
	{
		free_tree(&(*tree)->redirect->prev);
		free((*tree)->redirect);
	}
	free(*tree);
  *tree = NULL;
}

void	ft_free(t_mini *minishell)
{
	free_list(&minishell->list);
	free_tree(&minishell->tree);
}
