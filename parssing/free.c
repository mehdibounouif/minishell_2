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
	while (*list)
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
    {
		  free(tmp->content);
		  free(tmp);
      tmp = NULL;
    }
		tmp = *list;
	}
	free(list);
}

void	free_tree(t_tree **tree)
{
  if (!tree || !*tree)
    return ;
	if ((*tree)->type == COMMAND_NODE)
	{
		//free((*tree)->command->command);
		free_str((*tree)->command->args);
    (*tree)->command->command = NULL;
		free((*tree)->command);
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
    free((*tree)->redirect->file);
    free((*tree)->redirect->redirect);
		free_tree(&(*tree)->redirect->prev);
		free((*tree)->redirect);
	}
	else if ((*tree)->type == OR_NODE)
	{
		free_tree(&(*tree)->orr->left);
		free_tree(&(*tree)->orr->right);
		free((*tree)->orr);
		(*tree)->orr = NULL;
	}
	else if ((*tree)->type == AND_NODE)
	{
		free_tree(&(*tree)->andd->left);
		free_tree(&(*tree)->andd->right);
		free((*tree)->andd);
		(*tree)->andd = NULL;
	}
	free(*tree);
  *tree = NULL;
}

void	ft_free(t_node **list, t_tree **tree)
{
	free_tree(tree);
	free_list(list);
}
