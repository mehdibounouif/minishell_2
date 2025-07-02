/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 07:52:30 by mbounoui          #+#    #+#             */
/*   Updated: 2025/05/29 10:02:11 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	tokenize(char *cmd, t_node **list)
{
	t_node	*next;
	int		i;
	int		flag;

	next = NULL;
	i = 0;
	while (cmd[i])
	{
		while (is_space(cmd[i]))
			i++;
		flag = check_real_sep(cmd, i);
		next = get_token(cmd, &i);
		if (!next)
			return (0);
		add_back(list, next);
		token_type(next, flag);
	}
	return (1);
}
