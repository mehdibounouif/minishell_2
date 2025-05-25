/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 08:03:44 by mbounoui          #+#    #+#             */
/*   Updated: 2025/05/25 16:27:58 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(int c, char **v)
{
	t_node	*list;
	t_node	*tmp;
	t_tree	*tree;
	(void)c;
	
	list = NULL;
	tree = NULL;
	tokenize(v[1], &list);
	tmp = list;
	tree = pars_command(&tmp);
	print_list(list);
	return (0);
}
