/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_with_herdoc.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taha_laylay <taha_laylay@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:59:55 by mbounoui          #+#    #+#             */
/*   Updated: 2025/08/10 18:14:20 by taha_laylay      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../includes/minishell.h"

char	*get_last_herdoc(t_herdoc *list)
{
	if (!list)
		return (NULL);
	while (list->next)
	{
		list = list->next;
	}
	return (list->delimeter);
}

void	take_heredoc(t_tree *node, t_node *list)
{
	t_herdoc *h_node;

	h_node = ft_malloc(sizeof(t_herdoc), 1);
	h_node->quoted = 0;
	if (node->redirect->in)
	{
		node->redirect->hdc = 1;
		node->redirect->in = 0;
	}
	else
		node->redirect->hdc = 1;
	h_node->herdoc = ft_strdup(list->content);
	list = list->next;
	h_node->delimeter = ft_strdup(list->content);
	if (list->contain_quoted || list->between_quoted)
		h_node->quoted = 1;
	h_node->next = NULL;
	add_back3(&node->redirect->herdoc, h_node);
}

void	collect_herdoc(t_tree *node, t_node *list)
{
	while (list && list->type != PIPE)
	{
		if (list->type == R_IN)
			node->redirect->in_count++;
		else if (list->type == R_OUT || list->type == R_APPEND)
			node->redirect->out_count++;
		if (list->type == HEREDOC && list->next)
			take_heredoc(node, list);
		if (list->type == R_IN && node->redirect->hdc)
		{
			node->redirect->hdc = 0;
			node->redirect->in = 1;
		}
		else if (list->type == R_IN && !node->redirect->hdc)
			node->redirect->in = 1;
		list = list->next;
	}
}
