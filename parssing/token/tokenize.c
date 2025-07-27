/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 07:52:30 by mbounoui          #+#    #+#             */
/*   Updated: 2025/07/27 08:22:11 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	tokenize(char *cmd, t_node **list)
{
	t_node	*token;
	int		i;
	int		flag;

	token = NULL;
	i = 0;
	while (cmd[i])
	{
		while (is_space(cmd[i]))
			i++;
		if (!cmd[i])
			break ;
		flag = check_real_sep(cmd, i);
		token = get_token(cmd, &i);
		token->next = NULL;
		add_back(list, token);
		token_type(token, flag);
	}
}
