/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 19:07:26 by mbounoui          #+#    #+#             */
/*   Updated: 2025/07/27 21:28:29 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_dollar(char *cmd, int i)
{
	if (cmd[i] == '$' && cmd[i + 1] && check_quotes(cmd, i) != 2)
		return (1);
	return (0);
}

int	get_env_len(char *cmd, int i)
{
	int	j;

	j = 0;
	while (cmd[i] && (ft_isalpha(cmd[i]) || cmd[i] == '_'))
	{
		i++;
		j++;
	}
	return (j);
}
