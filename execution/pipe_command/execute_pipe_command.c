/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe_command.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moraouf <moraouf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 14:40:37 by mbounoui          #+#    #+#             */
/*   Updated: 2025/08/09 16:02:13 by moraouf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <unistd.h>

int	left(pid_t p[2], t_env **env, t_tree *tree, char **envp)
{
	pid_t	left;
	left = fork();
  if (left < 0)
  {
    ft_free_garbage(ft_function());
    free_env(*env);
    exit(1);
  }
	else if (left == 0)
	{
		// first child process;
    if (dup2(p[1], 1) == -1)
    {
	    perror("dup2 failed");
      ft_free_garbage(ft_function());
      free_env(*env);
	    exit(1);
    }
		close(p[1]);
    close(p[0]);
		execute_full_command(tree->pipe->left, env, envp, 1);
		exit(global(-1));
	}
	else
		return (left);
}

int	right(int *p, t_env **env, t_tree *tree, char **envp)
{
	pid_t	right;

	right = fork();
  if (right < 0)
  {
    ft_free_garbage(ft_function());
    free_env(*env);
    exit(1);
  }
	else if (right == 0)
	{
		// second child process; // close write side of pipe;
    if (dup2(p[0], 0) == -1)
    {
	    perror("dup2 failed");
      ft_free_garbage(ft_function());
      free_env(*env);
	    exit(1);
    }
		close(p[0]);
    close(p[1]);
		execute_full_command(tree->pipe->right, env, envp, 1);
		exit(global(-1));
	}
	else
		return (right);
}


void	execute_pipe_node(t_tree *tree, t_env *env, char **envp)
{
	int		p[2];
	int		status;
	pid_t l;
	pid_t r;

	if (pipe(p) == -1)
  {
    ft_free_garbage(ft_function());
    free_env(env);
    exit(1);
  }
	l = left(p, &env, tree, envp);
	r = right(p, &env, tree, envp);
	close(p[0]);
	close(p[1]);
	waitpid(l, NULL, 0);
	waitpid(r, &status, 0);
	global(WEXITSTATUS(status));
}
