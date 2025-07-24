/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moraouf <moraouf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 16:42:03 by moraouf           #+#    #+#             */
/*   Updated: 2025/07/24 22:29:18 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	echo_command(char **args)
{
	int	i;
	int	j;
	int	len;
	int	newline;

	i = 1;
	j = 1;
	newline = 1;
	if (args[1] && !ft_strncmp(args[1], "-n", 2))
	{
		len = ft_strlen(args[1]);
		while (args[1][j])
		{
			if (args[1][j] != 'n')
				break ;
			j++;
		}
		if (j == len)
		{
			newline = 0;
			i = 2;
		}
	}
	while (args[i])
	{
		ft_putstr_fd(args[i], 1);
		if (args[i + 1])
			ft_putchar_fd(' ', 1);
		i++;
	}
	if (newline)
	{
		ft_putchar_fd('\n', 1);
	}
	global(0);
	return (0);
}
