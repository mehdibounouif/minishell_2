/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 21:40:36 by mbounoui          #+#    #+#             */
/*   Updated: 2025/07/27 09:03:06 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_gcollect	**ft_function(void)
{
	static t_gcollect	*head;

	return (&head);
}

static t_gcollect	*new_node_gbc(void *new_gb)
{
	t_gcollect	*new;

	new = malloc(sizeof(t_gcollect));
	if (!new)
		return (NULL);
	new->value = new_gb;
	new->next = NULL;
	return (new);
}

static void	add_back_gb(t_gcollect **gb, t_gcollect *new)
{
	t_gcollect	*tmp;

	tmp = *gb;
	if (!*gb)
	{
		*gb = new;
		return ;
	}
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

void	ft_free_garbage(t_gcollect **list)
{
	t_gcollect	*tmp;

	while (*list)
	{
		tmp = (*list)->next;
		free((*list)->value);
		free(*list);
		*list = tmp;
	}
}

void	*ft_malloc(int type, int size)
{
	void		*result;
	t_gcollect	**head;

	head = ft_function();
	result = malloc(type * size);
	if (!result)
	{
		ft_free_garbage(ft_function());
		exit(global(-1));
	}
	add_back_gb(head, new_node_gbc(result));
	return (result);
}
