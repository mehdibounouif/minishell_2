/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taha_laylay <taha_laylay@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 08:03:44 by mbounoui          #+#    #+#             */
/*   Updated: 2025/08/09 23:51:22 by taha_laylay      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */



#include "../includes/minishell.h"

int global(int state)
{
	static int value;
	if(state != -1)
		value = state;
	return value;
}

static int	check_heredoc(t_tree *tree)
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

static int handel_heredocs(t_tree *tree, t_env *envp)
{
  int flag;

  flag = 0;
  if (check_heredoc(tree))
    if (!open_herdocs(&flag, tree, envp))
    {
      if (flag) // retourner au prompt
        return (0);
      else // Erreur système, quitter
      {
        ft_free_garbage(ft_function());
        exit(global(-1));
      }
    }
  if (flag) // Signal reçu pendant le heredoc, ne pas exécuter
    return (0);
  return (1);
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
		//  print_ast(minishell.tree, 0);
     if (!handel_heredocs(minishell.tree, envp))
      continue;
		sig_ctrl(1); // Set execution mode
		execute_full_command(minishell.tree, &envp, env, 0);
	  sig_ctrl(0); // Back to interactive mode
	}
  ft_free_garbage(ft_function());
	exit(global(-1));
}