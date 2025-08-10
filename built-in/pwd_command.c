/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd_command.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taha_laylay <taha_laylay@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 16:55:26 by moraouf           #+#    #+#             */
/*   Updated: 2025/08/10 00:29:39 by taha_laylay      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	pwd_command(void)
{
	char	cwd[4096];

	if (!getcwd(cwd, sizeof(cwd)))
	{
		if (*(back_up()) != NULL)
		{
			ft_putstr_fd(*(back_up()), 1);
			ft_putchar_fd('\n', 1);
			return (EXIT_SUCCESS);
		}
		else
		{
			ft_putstr_fd("minishell: pwd: error retrieving current directory\n",
				2);
			return (EXIT_FAILURE);
		}
	}
	ft_putstr_fd(cwd, 1);
	ft_putchar_fd('\n', 1);
	return (global(0));
}
