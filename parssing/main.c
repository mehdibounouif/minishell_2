/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moraouf <moraouf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 08:03:44 by mbounoui          #+#    #+#             */
/*   Updated: 2025/07/13 14:59:20 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(int c, char **v, char **env) 
{
	t_mini  minishell;
	(void)v;

	if (c != 1)
	{
		ft_putendl_fd("This program does not accept arguments", 2);
		exit(0);
	}
	ft_bzero(&minishell, sizeof(t_mini));
	// Initialize env only once!
	get_env(&minishell, env);
	while (1)
	{
		//handle signals
		handle_signal();
		// PARSSING
		if (!readline_and_parssing(&minishell))
			continue;
		open_herdocs(minishell.tree, minishell.env);
		//print_redirection_data(minishell.tree);
		//print_herdoc(minishell.tree);
		execute_full_command(minishell.tree, minishell.env, env);
    		// PRINT TREE
		//print_tree_unicode(minishell.tree, "", 1);
    		// FREE
		//ft_free(&minishell);
	}
	exit(ret); // add exit_code ;;;
}
