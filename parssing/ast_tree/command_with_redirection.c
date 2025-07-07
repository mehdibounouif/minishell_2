/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_with_redirection.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 14:40:57 by mbounoui          #+#    #+#             */
/*   Updated: 2025/07/07 15:33:42 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_tree	*parss_redirection_in_start(t_node **list, t_env *env)
{
	t_tree	*redirect_node;
	t_tree	*node;

	(void)env;
	redirect_node = malloc(sizeof(t_tree));
	if (!redirect_node)
		return (NULL);
	redirect_node->redirect = malloc(sizeof(t_redirection));
	if (!redirect_node->redirect)
	{
		free(redirect_node);
		return (NULL);
	}
	redirect_node->type = REDIRECT_NODE;
	redirect_node->redirect->redirect = (*list)->content;
	redirect_node->redirect->redirection_type = (*list)->type;
	printf("%d\n", (*list)->type);
	*list = (*list)->next;
	redirect_node->redirect->file = (*list)->content;
	*list = (*list)->next;
	node = pars_one_side(list, env);
	redirect_node->redirect->prev = node;
	return (redirect_node);
}

t_tree  *parss_redirection(t_tree *node, t_node **list, t_env *env)
{
	t_tree  *redirect_node;

	while (*list && (*list)->type == HEREDOC)
	{
		redirect_node = parss_herdoc(node, list, env);
		*list = (*list)->next;
	}
	if (redirect_node)
		return redirect_node;
	(void)env;
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

	return (redirect_node);
}

