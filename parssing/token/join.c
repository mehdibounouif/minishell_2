/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 11:41:03 by mbounoui          #+#    #+#             */
/*   Updated: 2025/08/02 20:30:03 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void remove_node(t_node **head, t_node **end)
{
	t_node *tmp;
	tmp = *head;
	while (tmp != *end)
	{
		t_node *next = tmp->next;
		free(tmp->content);
		free(tmp);
		tmp = next;
	}
}

static char *join_nodes_content(t_node *start, t_node *end)
{
    char *joined = NULL;
    t_node *tmp = start;

    while (tmp != end)
    {
        joined = ft_strjoin(joined, tmp->content);
        tmp = tmp->next;
    }
    return joined;
}

static void replace_b_space_sequence(t_node **head, t_node *start, t_node *end)
{
    char *joined = join_nodes_content(start, end);
    t_node *new_node = create_node(joined, 0, 0);
    if (!new_node)
        return;

    new_node->prev = start->prev;
    new_node->next = end;
    if (start->prev)
        start->prev->next = new_node;
    else
        *head = new_node;
    if (end)
        end->prev = new_node;

    remove_node(&start, &end);
}

void join_b_space_nodes(t_node **head)
{
    t_node  (*tmp), (*prev), (*start), (*end);

    tmp = *head;
    while (tmp)
    {
        if (tmp->b_space == 1)
        {
            prev = tmp->prev;
            start = tmp;
            end = tmp;
            while (end && end->b_space == 1)
                end = end->next;
            if (end)
                end = end->next;
            replace_b_space_sequence(head, start, end);
            if (prev)
                tmp = prev->next;
            else
                tmp = *head;
        }
        else
            tmp = tmp->next;
    }
}
