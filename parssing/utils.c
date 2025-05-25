/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 08:01:06 by mbounoui          #+#    #+#             */
/*   Updated: 2025/05/25 10:23:05 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_space(char c)
{
	if ((c >= 9 && c <= 13) || c == 32)
		return (1);
	return (0);
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
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = node;
		node->prev = tmp;
	}
}

void	print_list(t_node *list)
{
	while (list)
	{
		printf(" %s => ", list->content);
		switch (list->type) {
		case PIPE:
				printf("Type pipe (|)\n"); break;
		case WORD:
				printf("Type WORD\n"); break;
		case REDIRECTION_OUT:
				printf("Type redirection output (>)\n"); break;
		case HEREDOC:
				printf("Type heredoc (<<)\n"); break;
		case REDIRECTION_IN:
				printf("Type redirection input (<)\n"); break;
		case REDIRECTION_ERR:
				printf("Type redirection error (2)\n"); break;
		case REDIRECTION_APPEND:
				printf("Type redirection append (>>)\n"); break;
		case RUN_BACKGROUND:
				printf("Type run in background (&)\n"); break;
		}
		printf("\n");
		list = list->next;
	}
}
