/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moraouf <moraouf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 08:03:44 by mbounoui          #+#    #+#             */
/*   Updated: 2025/08/02 14:37:02 by mbounoui         ###   ########.fr       */
/*   Updated: 2025/07/26 13:51:28 by moraouf          ###   ########.fr       */
/*   Updated: 2025/07/30 11:31:08 by moraouf          ###   ########.fr       */
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

int	heredoc(int *flag, t_mini minishell, t_env *envp)
{
	pid_t pid;
	int status;

	sig_ctrl(1); //mode execution
	pid = fork();
	if(pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		//	signal(SIGQUIT, SIG_DFL);
		open_herdocs(minishell.tree, envp);
    exit(global(-1));
	}
	else if(pid > 0)
	{
		waitpid(pid, &status, 0);
    /// printf("%s\n", WIFSIGNALED(status) ? "yes" : "no");
		*flag = ft_return_signal(status);
		sig_ctrl(0);
		//ft_free_garbage(ft_function());
	}
	return (1);
}

int	check_heredoc(t_tree *tree)
{
	if (tree->type == REDIRECT_NODE)
	{
		if (tree->redirect->herdoc)
			return (1);
	}
	if (tree->type == PIPE_NODE)
	{
		return (check_heredoc(tree->pipe->left));
		return (check_heredoc(tree->pipe->right));
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
	if (!get_env(&envp, env))
		exit (0);
	global(0); //Initial state, interactive mode 
	while (1)
	{
		handle_signal();
		if (!readline_and_parssing(&minishell, envp))
			continue;
		print_ast(minishell.tree, 0);
		int flag = 0;
		if (check_heredoc(minishell.tree))
			if (!heredoc(&flag, minishell, envp))
			{
			  ft_free_garbage(ft_function());
				exit(global(-1));
			}
		if (flag)
			continue;
		sig_ctrl(1); // Set execution mode
		execute_full_command(minishell.tree, envp, env, 0, NULL);
	  sig_ctrl(0); // Back to interactive mode
	}
  ft_free_garbage(ft_function());
	exit(global(-1));
	//return (0);
}
