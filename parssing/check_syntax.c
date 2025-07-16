/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 11:51:06 by mbounoui          #+#    #+#             */
/*   Updated: 2025/07/08 11:51:08 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int check_sides(t_node *list)
{
	if (list->type == PIPE || list->content[0] == '#')
		return (1);
	while(list->next)
		list = list->next;
	if (!list->between_quoted && list->type == PIPE)
		return (1);
	return (0);
}

int     check_syntax(t_mini *mini, t_node *list)
{
  (void)mini;
	if (check_sides(list))
	{
		ft_putendl_fd("bash: syntax error near unexpected token |", 2);
//		mini->ret = 258;
		return (0);
	}
	while (list)
	{
		if ((!list->between_quoted && list->type == PIPE && list->next->type == PIPE)
			|| (is_redirection(list) && !list->between_quoted && (!list->next || list->next->type != WORD)))
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
//			mini->ret = 258;
			return (0);
		}
		list = list->next;
	}
	return (1);
}
