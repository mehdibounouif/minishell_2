/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd_command.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moraouf <moraouf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 16:55:26 by moraouf           #+#    #+#             */
/*   Updated: 2025/08/02 22:17:14 by moraouf          ###   ########.fr       */
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
			ft_free_garbage(ft_function());
			return (EXIT_SUCCESS);
		}
		else
		{
			ft_putstr_fd("minishell: pwd: error retrieving current directory\n",
					2);
			ft_free_garbage(ft_function());
			return (EXIT_FAILURE);
		}
	}
	ft_putstr_fd(cwd, 1);
	ft_putchar_fd('\n', 1);
	// free(cwd);
	ft_free_garbage(ft_function());
	return (EXIT_SUCCESS);
}
