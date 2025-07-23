/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moraouf <moraouf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 08:03:44 by mbounoui          #+#    #+#             */
/*   Updated: 2025/07/23 11:38:30 by mbounoui         ###   ########.fr       */
/*   Updated: 2025/07/23 10:45:20 by moraouf          ###   ########.fr       */
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


void	open_her(t_mini minishell)
{
	pid_t pid;
 	int status;

	sig_ctrl(1); //mode execution
	pid = fork();
	if(pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		open_herdocs(minishell.tree, minishell.env);
		exit(0);
	}
	else if(pid > 0)
	{
		// Parent process - ignore signals while child runs
		// signal(SIGINT, SIG_IGN);
		// signal(SIGQUIT, SIG_IGN);
		
		waitpid(pid, &status, 0);
//		printf("%s\n",WIFSIGNALED(status)?"yes":"no");
		ft_return_signal(status);
		sig_ctrl(0);
		// Don't call handle_signal() here - it will be called in main loop
	}
	// global(0); // reset state , interactive mode 
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
	global(0); //Initial state, interactive mode 
	while (1)
	{
		handle_signal();
		if (!readline_and_parssing(&minishell))
			continue;
		
		// Process heredocs first if needed
		open_her(minishell);
		
		// start execution mode before running the command
		sig_ctrl(1); // Set execution mode
		execute_full_command(minishell.tree, minishell.env, env);
		sig_ctrl(0); // Back to interactive mode
	//	print_ast(minishell.tree, 0);
	//	ft_free(&minishell);
	}
	//free_env(minishell.env);
	exit(global(-1));
  return (0);
}
