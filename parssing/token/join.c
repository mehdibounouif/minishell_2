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

char *strjoin_and_free(char *s1, char *s2)
{
	size_t  len1 = 0;
	size_t  len2 = 0;
	if (s1)
		len1 = ft_strlen(s1);
	if (s2)    
		len2 = ft_strlen(s2);
    char *joined = malloc(len1 + len2 + 1);
    if (!joined)
        return NULL;
    if (s1)
        ft_strlcpy(joined, s1, len1 + 1);
    if (s2)
        ft_strlcpy(joined + len1, s2, len2 + 1);
    free(s1);
    return joined;
}

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

void join_b_space_nodes(t_node **head)
{
  t_node (*new_node),(*tmp),(*start),(*end);
	char *joined;

  tmp = *head;
	while (tmp)
	{
		if (tmp->b_space == 1)
		{
			start = tmp;
			end = tmp;
			while (end && end->b_space == 1)
				end = end->next;
			if (end)
				end = end->next;
			tmp = start;
			joined = NULL;
			while (tmp != end)
			{
				joined = ft_strjoin(joined, tmp->content);
				tmp = tmp->next;
			}
			new_node = create_node(joined, 0);
			free(joined);
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
			tmp = new_node->next;
		}
		else
			tmp = tmp->next;
	}
}
