/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 11:51:06 by mbounoui          #+#    #+#             */
/*   Updated: 2025/08/12 17:41:30 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	check_sides(t_node *list, int end, int start)
{
	if (list && list->type == PIPE && !list->between_quoted && !start)
		return (1);
	while (list && list->next)
		list = list->next;
	if (list && !list->between_quoted && list->type == PIPE && !end)
		return (1);
	return (0);
}

int	double_pipe(t_node *list)
{
	while (list)
	{
		if ((list->next && !list->between_quoted && list->type == PIPE
				&& list->next->type == PIPE)
        || (list->type == PIPE && !list->next && !list->prev))
    {
      ft_putendl_fd("bash: syntax error near unexpected token |", 2);
			return (1);
    }
		list = list->next;
	}
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

int	check_syntax(t_node *list, int end, int start)
{
	if (check_sides(list, end, start))
	{
		ft_putendl_fd("bash: syntax error near unexpected token |", 2);
		global(2);
		return (0);
	}
	if (!syntax(list))
	{
		global(2);
		return (0);
	}
	return (1);
}
