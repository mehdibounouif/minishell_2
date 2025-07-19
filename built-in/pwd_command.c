/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd_command.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moraouf <moraouf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 16:55:26 by moraouf           #+#    #+#             */
/*   Updated: 2025/07/19 17:20:00 by moraouf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int pwd_command(t_env *env, char **args)
{
	char *cwd;
	char *pwd_env;


	(void)args;
	// if (args[1] != NULL)
	// {
	// 	ft_putstr_fd("minishell: pwd: too many arguments\n", STDERR_FILENO);
	// 	return (EXIT_FAILURE);
	// }
	
	// First try to get PWD from environment (logical path)
	pwd_env = get_env_value(env, "PWD");
	if (pwd_env)
	{
		// Verify that PWD is valid by checking if it points to the same directory
		cwd = getcwd(NULL, 0);
		if (cwd)
		{
			// Compare the paths - use PWD if it's valid
			struct stat pwd_stat, cwd_stat;
			if (stat(pwd_env, &pwd_stat) == 0 && stat(cwd, &cwd_stat) == 0)
			{
				if (pwd_stat.st_dev == cwd_stat.st_dev && pwd_stat.st_ino == cwd_stat.st_ino)
				{
					ft_putstr_fd(pwd_env, 1);
					ft_putchar_fd('\n', 1);
					free(cwd);
					return (EXIT_SUCCESS);
				}
			}
			free(cwd);
		}
	}
	
	// Fall back to getcwd() if PWD is not available or invalid
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		ft_putstr_fd("minishell: pwd: error retrieving current directory\n", 2);
		return (EXIT_FAILURE);
	}
	ft_putstr_fd(cwd, 1);
	ft_putchar_fd('\n', 1);
	free(cwd);
	return (EXIT_SUCCESS);
}