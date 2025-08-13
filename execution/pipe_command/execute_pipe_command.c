/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe_command.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moraouf <moraouf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 14:40:37 by mbounoui          #+#    #+#             */
/*   Updated: 2025/08/13 01:01:04 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <unistd.h>

int	left(pid_t p[2], t_env **env, t_tree *tree)
{
	pid_t	left;

	left = fork();
	if (left < 0)
		protect(*env, "Fork failed");
	else if (left == 0)
	{
		if (dup2(p[1], 1) == -1)
			protect(*env, "Dup2 failed");
		close(p[1]);
		close(p[0]);
		if (tree->pipe->left)
		{
			execute_full_command(tree->pipe->left, env, 1);
			exit(global(-1));
		}	
		else
		{
			exit(0);
		}
	}
	return (left);
}

int	right(int *p, t_env **env, t_tree *tree)
{
	pid_t	right;

	right = fork();
	if (right < 0)
		protect(*env, "Fork failed");
	else if (right == 0)
	{
		if (dup2(p[0], 0) == -1)
			protect(*env, "Dup2 failed");
		close(p[0]);
		close(p[1]);
		if (tree->pipe->right)
		{
			execute_full_command(tree->pipe->right, env, 1);
			exit(global(-1));
		}
		else
			exit(0);
	}
	return (right);
}

void	execute_pipe_node(t_tree *tree, t_env *env)
{
	int		p[2];
	int		status;
	pid_t	l;
	pid_t	r;

	status = 0;
	if (pipe(p) == -1)
		protect(env, "Pipe failed");
	l = left(p, &env, tree);
	r = right(p, &env, tree);
	close(p[0]);
	close(p[1]);
	if (waitpid(l, NULL, 0) == -1 || waitpid(r, &status, 0) == -1)
		protect(env, "Waitpid failed");
	global(WEXITSTATUS(status));
}
