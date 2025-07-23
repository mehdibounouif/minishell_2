/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moraouf <moraouf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 08:03:44 by mbounoui          #+#    #+#             */
/*   Updated: 2025/07/23 22:29:19 by mbounoui         ###   ########.fr       */
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


void	open_her(t_mini minishell, t_env *envp)
{
	pid_t pid;
 	int status;

	sig_ctrl(1); //mode execution
	pid = fork();
	if(pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		open_herdocs(minishell.tree, envp);
	//	exit(0);
	}
	else if(pid > 0)
	{
		waitpid(pid, &status, 0);
		// printf("%s\n", WIFSIGNALED(status) ? "yes" : "no");
		ft_return_signal(status);
		sig_ctrl(0);
	}
}
int	check_heredoc(t_tree *tree)
{
	if (tree->type == REDIRECT_NODE)
	{
		if (tree->redirect->herdoc)
			return (1);
	}
	if (tree->type == PIPE)
	{
		check_heredoc(tree->pipe->left);
		check_heredoc(tree->pipe->right);
	}
	return (0);
}
int	main(int c, char **v __attribute__((unused)), char **env)
{
	t_mini  minishell;
	t_env *envp;

	if (c != 1)
	{
		ft_putendl_fd("This program does not accept arguments", 2);
		exit(0);
	}
	envp = NULL;
	ft_bzero(&minishell, sizeof(t_mini));
	get_env(&envp, env);
	global(0); //Initial state, interactive mode 
	while (1)
	{
		handle_signal();
		if (!readline_and_parssing(&minishell, envp))
			continue;
		if (check_heredoc(minishell.tree))
			open_her(minishell, envp);
		
		sig_ctrl(1); // Set execution mode
		execute_full_command(minishell.tree, envp, env);
		sig_ctrl(0); // Back to interactive mode
	//	print_ast(minishell.tree, 0);
	}
	free_env(envp);
	exit(global(-1));
	return (0);
}
