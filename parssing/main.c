/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taha_laylay <taha_laylay@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 08:03:44 by mbounoui          #+#    #+#             */
/*   Updated: 2025/08/09 23:40:20 by taha_laylay      ###   ########.fr       */
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

// int	heredoc(int *flag, t_mini minishell, t_env *envp)
// {
// 	pid_t pid;
// 	int status;

// 	sig_ctrl(1); //mode execution
// 	pid = fork();
// 	if(pid == 0)
// 	{
// 		signal(SIGINT, SIG_DFL);
// 		//	signal(SIGQUIT, SIG_DFL);
// 		open_herdocs(flag, minishell.tree, envp);
//     exit(global(-1));
// 	}
// 	else if(pid > 0)
// 	{
// 		waitpid(pid, &status, 0);
//     /// printf("%s\n", WIFSIGNALED(status) ? "yes" : "no");
// 		*flag = ft_return_signal(status);
// 		sig_ctrl(0);
// 		//ft_free_garbage(ft_function());
// 	}
// 	return (1);
// }

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
		// print_ast(minishell.tree, 0);
	  int flag_sig = 0;
		if (check_heredoc(minishell.tree))
			if (!open_herdocs(&flag_sig, minishell.tree, envp))
			{
<<<<<<< HEAD
				if (flag) // retourner au prompt
					continue;
				else // Erreur système, quitter
				{
					ft_free_garbage(ft_function());
					exit(global(-1));
				}
			}
		 if (flag) // Signal reçu pendant le heredoc, ne pas exécuter
		 	continue;
=======
        free_env(envp);
			  ft_free_garbage(ft_function());
				exit(global(-1));
			}
		if (flag_sig) //always is true so is never got to execution stage (check why)
			continue;
>>>>>>> 328edac0bae6d505a147abcc2a40da78ac46ad2c
		sig_ctrl(1); // Set execution mode
		execute_full_command(minishell.tree, &envp, env, 0);
	  sig_ctrl(0); // Back to interactive mode
	}
  free_env(envp);
  ft_free_garbage(ft_function());
	exit(global(-1));
}
