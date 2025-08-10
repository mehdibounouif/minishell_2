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

// Helper: join all node->content from start up to (but not including) end
// static char *join_nodes_content(t_node *start, t_node *end)
// {
//     char *joined = NULL;
//     t_node *tmp = start;

//     while (tmp != end)
//     {
//         joined = ft_strjoin(joined, tmp->content);
//         tmp = tmp->next;
//     }
//     return joined;
// }

// // Main: find b_space sequences, replace them with one joined node
// void join_b_space_nodes(t_node **head)
// {
//     t_node *tmp = *head;

//     while (tmp)
//     {
//         if (tmp->b_space == 1)
//         {
//             t_node *start = tmp;
//             t_node *end = tmp;
//             while (end && end->b_space == 1)
//                 end = end->next;

//             if (end)
//                 end = end->next; // move past non-b_space node
//             char *joined = join_nodes_content(start, end);
//             t_node *new_node = create_node(joined, 0, 0);
//             if (!new_node)
//                 return;
//             new_node->prev = start->prev;
//             new_node->next = end;
//             if (start->prev)
//                 start->prev->next = new_node;
//             else
//                 *head = new_node;
//             if (end)
//                 end->prev = new_node;
//             remove_node(&start, &end);

//             tmp = new_node->next;
//         }
//         tmp = tmp->next;
//     }
// }


// void join_b_space_nodes(t_node **head)
// {
//   t_node *new_node;
//   t_node *tmp;
//   t_node *start;
//   t_node *end;
// 	char *joined;

//   tmp = *head;
// 	while (tmp)
// 	{
// 		if (tmp->b_space == 1)
// 		{
// 			start = tmp;
// 			end = tmp;
// 			while (end && end->b_space == 1)
// 				end = end->next;
// 			if (end)
// 				end = end->next;
// 			tmp = start;
// 			joined = NULL;
// 			while (tmp != end)
// 			{
// 				joined = ft_strjoin(joined, tmp->content);
// 				tmp = tmp->next;
// 			}
// 			new_node = create_node(joined, 0, 0);
// 			if (!new_node)
// 				return;
// 			new_node->prev = start->prev;
// 			new_node->next = end;
// 			if (start->prev)
// 				start->prev->next = new_node;
// 			else
// 				*head = new_node;
// 			if (end)
// 				end->prev = new_node;

// 			remove_node(&start, &end);
// 			tmp = new_node->next;
// 		}
// 		else
// 			tmp = tmp->next;
// 	}
// }


// Joins content of nodes from start to end (exclusive)
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

// Creates replacement node, rewires list, removes old sequence
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

// Finds b_space sequences and replaces them
void join_b_space_nodes(t_node **head)
{
    t_node *tmp = *head;
    while (tmp)
    {
        if (tmp->b_space == 1)
        {
            t_node *start = tmp;
            t_node *end = tmp;
            while (end && end->b_space == 1)
                end = end->next;
            if (end)
                end = end->next;
            replace_b_space_sequence(head, start, end);
            tmp = (start->prev) ? start->prev->next : *head; // continue after replacement
        }
        else
            tmp = tmp->next;
    }
}
