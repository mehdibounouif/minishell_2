/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moraouf <moraouf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 08:03:44 by mbounoui          #+#    #+#             */
/*   Updated: 2025/07/21 17:23:25 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <sys/wait.h>
#include <unistd.h>

int global(int state)
{
	static int value;
	if(state != -1)
		value = state;
	return value;
}

void	open_her(t_mini minishell)
{
	pid_t pid;
	pid = fork();
	if (pid == 0)
		open_herdocs(minishell.tree, minishell.env);
	else
		waitpid(pid, NULL, 0);
}

int	main(int c, char **v __attribute__((unused)), char **env)
{
	t_mini  minishell;

	if (c != 1)
	{
		ft_putendl_fd("This program does not accept arguments", 2);
		exit(0);
	}
	ft_bzero(&minishell, sizeof(t_mini));
	get_env(&minishell, env);
	while (1)
	{
		handle_signal();
		if (!readline_and_parssing(&minishell))
			continue;
		open_her(minishell);
		execute_full_command(minishell.tree, minishell.env, env);
		//print_ast(minishell.tree, 0);
		ft_free(&minishell);
	}
	exit(minishell.tree->ret);
  return (0);
}
