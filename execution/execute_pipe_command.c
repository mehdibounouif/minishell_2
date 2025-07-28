/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe_command.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 14:40:37 by mbounoui          #+#    #+#             */
/*   Updated: 2025/07/28 08:56:25 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <stdlib.h>

void	execute_pipe_node(t_tree *tree, t_env *env, char **envp)
{
	int	p[2];
	pid_t	left;
	pid_t	right;
	int	status;

	pipe(p);
	left = fork();
	if (left == 0)
	{
		// first child process;
		close(p[0]); // close read side of pipe;
		dup2(p[1], 1); 
		close(p[1]);
		execute_full_command(tree->pipe->left, env, envp);
		ft_free_garbage(ft_function());
		free_env(env);
		exit(global(-1));
	}
	right = fork();
	if (right == 0)
	{
		// second child process;
		close(p[1]); // close write side of pipe;
		dup2(p[0], 0); 
		close(p[0]);
		execute_full_command(tree->pipe->right, env, envp);
		ft_free_garbage(ft_function());
		free_env(env);
		exit(global(-1));
	}
	close(p[0]);
	close(p[1]);
	waitpid(left, NULL, 0);
	waitpid(right, &status, 0);
//	ft_free_garbage(ft_function());
//	free_env(env);
	global(WEXITSTATUS(status));
}
