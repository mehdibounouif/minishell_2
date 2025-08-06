/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe_command.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 14:40:37 by mbounoui          #+#    #+#             */
/*   Updated: 2025/08/01 22:07:34 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <unistd.h>

int	left(pid_t p[2], t_env *env, t_tree *tree, char **envp)
{
	pid_t	left;
	left = fork();
	if (left == 0)
	{
		// first child process;
    if (dup2(p[1], 1) == -1)
    {
	    perror("dup2 failed");
	    exit(1);
    }
    close(1);
		close(p[1]);
    close(p[0]);
		execute_full_command(tree->pipe->left, env, envp, 1, p);
		exit(global(-1));
	}
	else
		return (left);
}

int	right(int *p, t_env *env, t_tree *tree, char **envp)
{
	pid_t	right;

	right = fork();
	if (right == 0)
	{
		// second child process; // close write side of pipe;
		dup2(p[0], 0);
    close(0);
		close(p[0]);
    close(p[1]);
		execute_full_command(tree->pipe->right, env, envp, 1, p);
		exit(global(-1));
	}
	else
  {
		return (right);
  }
}


void	execute_pipe_node(t_tree *tree, t_env *env, char **envp)
{
	int		p[2];
	int		status;
	pid_t l;
	pid_t r;

	pipe(p);
	l = left(p, env, tree, envp);
	r = right(p, env, tree, envp);
	close(p[0]);
	close(p[1]);
	waitpid(l, NULL, 0);
	waitpid(r, &status, 0);
	global(WEXITSTATUS(status));
}
