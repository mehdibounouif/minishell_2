/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 16:45:26 by mbounoui          #+#    #+#             */
/*   Updated: 2025/08/13 00:47:05 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	check_quotes(char *cmd, size_t i)
{
	size_t	j;
	int		flag;

	j = 0;
	flag = 0;
	while (cmd[j] && j != i)
	{
		if (flag == 0 && cmd[j] == '\"')
			flag = 1;
		else if (flag == 0 && cmd[j] == '\'')
			flag = 2;
		else if (flag == 1 && cmd[j] == '\"')
			flag = 0;
		else if (flag == 2 && cmd[j] == '\'')
			flag = 0;
		j++;
	}
	return (flag);
}

int	ft_arraylen(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
		i++;
	return (i);
}
