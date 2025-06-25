/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moraouf <moraouf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 08:03:44 by mbounoui          #+#    #+#             */
/*   Updated: 2025/06/25 14:37:34 by moraouf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


int	main(int c, char **v, char **env) 
{
	t_mini  *minishell;
	(void)c;
	(void)v;
	
	if (!(minishell = ft_calloc(1, sizeof(t_mini))))
		return 0;
	minishell->ret = 0;
	while (1)
	{
		//handle signals
		handle_signal();
		// PARSSING
		if (!readline_and_parssing(minishell, env))
			continue;
		execute_full_command(minishell->tree, env);
    		// PRINT TREE
		//print_tree_unicode(minishell->tree, "", 1);
    		// FREE
		ft_free(minishell);
		//free(cmd);
	}
	return (0);
}
