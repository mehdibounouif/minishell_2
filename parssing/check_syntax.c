/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 11:51:06 by mbounoui          #+#    #+#             */
/*   Updated: 2025/08/13 01:40:28 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	error(void)
{
	global(2);
	ft_putendl_fd("bash: syntax error near unexpected token |", 2);
}

int	check_pipe(t_node *list)
{
	t_node	*tmp;

	if (list && list->type == PIPE && !list->between_quoted)
		return (error(), 1);
	tmp = list;
	while (tmp)
	{
		if ((tmp->next && !tmp->between_quoted && tmp->type == PIPE
				&& tmp->next->type == PIPE)
			|| (tmp->type == PIPE && !tmp->next && !tmp->prev))
			return (error(), 1);
		tmp = tmp->next;
	}
	while (list && list->next)
		list = list->next;
	if (list && !list->between_quoted && list->type == PIPE)
		return (error(), 1);
	return (0);
}

int	syntax(t_node *list)
{
	while (list)
	{
		if (is_redirection(list) && !list->between_quoted && (!list->next
				|| list->next->type != WORD))
		{
			if (list->next)
			{
				ft_putstr_fd("bash: syntax error near unexpected token ", 2);
				ft_putendl_fd(list->next->content, 2);
			}
			else
			{
				ft_putstr_fd("bash: syntax error near unexpected token ", 2);
				ft_putendl_fd(list->content, 2);
			}
			return (0);
		}
		list = list->next;
	}
	return (1);
}

int	check_syntax(t_node *list)
{
	if (!syntax(list))
	{
		global(2);
		return (0);
	}
	return (1);
}
