/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 11:41:03 by mbounoui          #+#    #+#             */
/*   Updated: 2025/08/02 16:20:52 by mbounoui         ###   ########.fr       */
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

t_node *create_node(const char *content, int b_space)
{
    t_node *new = malloc(sizeof(t_node));
    if (!new)
        return NULL;
    new->content = ft_strdup(content);
    new->b_space = b_space;
	new->between_quoted = 1;
	new->contain_quoted = 1;
	new->type = 2;
    new->next = NULL;
    new->prev = NULL;
    return new;
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
	t_node *curr = *head;

	while (curr)
	{
		if (curr->b_space == 1)
		{
			t_node *start = curr;
			t_node *end = curr;

			while (end && end->b_space == 1)
				end = end->next;

			if (end)
				end = end->next;

			t_node *tmp = start;
			char *joined = NULL;
			while (tmp != end)
			{
				joined = strjoin_and_free(joined, tmp->content);
				tmp = tmp->next;
			}
			t_node *new_node = create_node(joined, 0);
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
			curr = new_node->next;
		}
		else
	{
			curr = curr->next;
		}
	}

}
/*
t_node *create_node(const char *content, int b_space)
{
    t_node *new = malloc(sizeof(t_node));
    if (!new)
        return NULL;
    new->content = strdup(content);
    new->b_space = b_space;
    new->next = NULL;
    new->prev = NULL;
    return new;
}

void remove_node(t_node **head, t_node *node)
{
    if (!node)
        return;
    if (node->prev)
        node->prev->next = node->next;
    else
        *head = node->next;
    if (node->next)
        node->next->prev = node->prev;
    free(node->content);
    free(node);
}

void join_flag_and_next(t_node **head)
{
    t_node *curr = *head;
    while (curr)
    {
        if (curr->b_space == 1 && curr->next)
        {
            t_node *next = curr->next;
            // Join curr->content + next->content
            char *joined = strjoin_and_free(strdup(curr->content), next->content);
            if (!joined)
                return;

            // Create new node with b_space = 0 (or keep curr->b_space if needed)
            t_node *new_node = create_node(joined, 0);
            free(joined);
            if (!new_node)
                return;

            // Insert new_node in place of curr
            new_node->prev = curr->prev;
            new_node->next = next->next;
            if (curr->prev)
                curr->prev->next = new_node;
            else
                *head = new_node;
            if (next->next)
                next->next->prev = new_node;

            // Remove curr and next
            remove_node(head, curr);
            remove_node(head, next);

            // Continue from new_node->next
            curr = new_node->next;
        }
        else
        {
            curr = curr->next;
        }
    }
}
*/
