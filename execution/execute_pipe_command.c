/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe_command.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 14:40:37 by mbounoui          #+#    #+#             */
/*   Updated: 2025/07/09 14:45:46 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	execute_pipe_node(t_tree *tree, t_env *env, char **envp)
{
	int	p[2];
	pid_t	left;
	pid_t	right;

	pipe(p);
	left = fork();
	if (left == 0)
	{
		// first child process;
		close(p[0]); // close read side of pipe;
		dup2(p[1], 1); 
		close(p[1]);
		execute_full_command(tree->pipe->left, env, envp);
		exit(1);
	}
	right = fork();
	if (right == 0)
	{
		// second child process;
		close(p[1]); // close write side of pipe;
		dup2(p[0], 0); 
		close(p[0]);
		execute_full_command(tree->pipe->right, env, envp);
		exit(1);
	}
	close(p[0]);
	close(p[1]);
	waitpid(left, NULL, 0);
	waitpid(right, NULL, 0);
}
