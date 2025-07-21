/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moraouf <moraouf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 08:03:44 by mbounoui          #+#    #+#             */
/*   Updated: 2025/07/21 09:38:48 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int global(int state)
{
	static int value;
	if(state != -1)
		value = state;
	return value;
}

int	main(int c, char **v __attribute__((unused)), char **env)
{
	t_mini  minishell;

	// check stdin stderr stdout with isatty();
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
		execute_full_command(minishell.tree, minishell.env, env);
		//print_ast(minishell.tree, 0);
		//printf("exit status = %d\n", minishell.tree->ret);
    	// FREE
		ft_free(&minishell);
	}
	exit(minishell.tree->ret);
  return (0);
}
