/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 15:50:04 by mbounoui          #+#    #+#             */
/*   Updated: 2025/08/11 15:50:27 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_node	*create_node2(const char *content, int b_space, int type, int b_q)
{
	t_node	*new;

	new = malloc(sizeof(t_node));
	new->content = ft_strdup1(content);
	new->b_space = b_space;
	new->between_quoted = b_q;
	new->contain_quoted = 1;
	new->type = type;
	new->next = NULL;
	new->prev = NULL;
	return (new);
}

void	add_back3(t_herdoc **list, t_herdoc *node)
{
	t_herdoc	*tmp;

	if (!list || !node)
		return ;
	if (!*list)
		*list = node;
	else
	{
		tmp = *list;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = node;
	}
}

void	add_back2(t_env **list, t_env *node)
{
	t_env	*tmp;

	if (!list || !node)
		return ;
	if (!*list)
		*list = node;
	else
	{
		tmp = *list;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = node;
	}
}

void	add_back1(t_files **list, t_files *node)
{
	t_files	*tmp;

	if (!list || !node)
		return ;
	if (!*list)
		*list = node;
	else
	{
		tmp = *list;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = node;
	}
}

void	add_back(t_node **list, t_node *node)
{
	t_node	*tmp;

	if (!list || !node)
		return ;
	if (!*list)
		*list = node;
	else
	{
		tmp = *list;
		while (tmp && tmp->next)
			tmp = tmp->next;
		tmp->next = node;
		node->prev = tmp;
	}
}
