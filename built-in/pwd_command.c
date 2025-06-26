/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd_command.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moraouf <moraouf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 16:55:26 by moraouf           #+#    #+#             */
/*   Updated: 2025/06/25 16:55:27 by moraouf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

	#include "../includes/minishell.h"
	
	int pwd_command(t_env *env, char **args)
	{
	    char    *cwd;
	    
	    (void)env;  

		 if (args[1] != NULL)
    {
        ft_putstr_fd("minishell: pwd: too many arguments\n", STDERR_FILENO);
        return (EXIT_FAILURE);
    }
	    
	    cwd = getcwd(NULL, 0);
		ft_putstr_fd(cwd,1);
		if(!cwd)
		{
			ft_putstr_fd("minishell: pwd:\n", 2);
			return (EXIT_FAILURE);
		}
	    // if(ft_putendl_fd(cwd, 1) == -1)
		// {
		// 	ft_putstr_fd("minishell: pwd: error retrieving current directory\n", 2);
		// 	return (EXIT_FAILURE);
		// }
	    free(cwd);
	    return (EXIT_SUCCESS);
	}