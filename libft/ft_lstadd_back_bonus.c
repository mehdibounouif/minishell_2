/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 17:41:40 by mbounoui          #+#    #+#             */
/*   Updated: 2025/05/19 07:58:46 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *node)
{
	t_list	*temp;

	if (lst)
	{
		if (*lst)
		{
			temp = *lst;
			while (temp->next != NULL)
				temp = temp->next;
			temp->next = node;
		}
		else
			*lst = node;
	}
}
