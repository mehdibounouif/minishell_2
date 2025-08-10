/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_front_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <moraouf@42.fr>                    +#+  +:+      
	+#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 16:20:35 by mbounoui          #+#    #+#             */
/*   Updated: 2025/05/19 07:58:44 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_front(t_list **lst, t_list *node)
{
	if (node == NULL)
		return ;
	node->next = *lst;
	*lst = node;
}
