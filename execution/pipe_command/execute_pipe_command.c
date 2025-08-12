/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe_command.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moraouf <moraouf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 14:40:37 by mbounoui          #+#    #+#             */
/*   Updated: 2025/08/11 23:45:20 by mbounoui         ###   ########.fr       */
/*   Updated: 2025/08/11 12:59:46 by moraouf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <unistd.h>

int	left(pid_t p[2], t_env **env, t_tree *tree, char **envp)
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
		execute_full_command(tree->pipe->left, env, envp, 1);
		exit(global(-1));
	}
	return (left);
}

int	right(int *p, t_env **env, t_tree *tree, char **envp)
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
		execute_full_command(tree->pipe->right, env, envp, 1);
		exit(global(-1));
	}
	return (right);
}

void	execute_pipe_node(t_tree *tree, t_env *env, char **envp)
{
	int		p[2];
	int		status = 0;
	pid_t	l;
	pid_t	r;

	status = 0;
	if (pipe(p) == -1)
		protect(env, "Pipe failed");
	l = left(p, &env, tree, envp);
	if (tree->pipe->right)
		r = right(p, &env, tree, envp);
	close(p[0]);
	close(p[1]);
	if (tree->pipe->right)
	{
		if (waitpid(l, NULL, 0) == -1 ||waitpid(r, &status, 0) == -1)
			protect(env, "Waitpid failed");
	}
	else 
		if (waitpid(l, &status, 0) == -1)
			protect(env, "Waitpid failed");
	global(WEXITSTATUS(status));
}
