/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moraouf <moraouf@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 16:42:03 by moraouf           #+#    #+#             */
/*   Updated: 2025/08/10 01:14:47 by moraouf      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	skip_n_flags(char **args, int *newline)
{
	int	i;
	int	j;

	i = 1;
	*newline = 1;
	while (args[i] && !ft_strncmp(args[i], "-n", 2))
	{
		j = 2;
		while (args[i][j] == 'n')
			j++;
		if (args[i][j] != '\0')
			break ;
		*newline = 0;
		i++;
	}
	return (i);
}

static void	print_args(char **args, int i)
{
	while (args[i])
	{
		ft_putstr_fd(args[i], 1);
		if (args[i + 1])
			ft_putchar_fd(' ', 1);
		i++;
	}
}

int	echo_command(char **args)
{
	int	i;
	int	newline;

	i = skip_n_flags(args, &newline);
	print_args(args, i);
	if (newline)
		ft_putchar_fd('\n', 1);
	return (global(0));
}
