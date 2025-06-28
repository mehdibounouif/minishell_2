#include "../includes/minishell.h"

void	execute_pipe_node(t_tree *tree, char **env)
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
		execute_full_command(tree->pipe->left, env);
		exit(1);
	}
	right = fork();
	if (right == 0)
	{
		// second child process;
		close(p[1]); // close write side of pipe;
		dup2(p[0], 0); 
		close(p[0]);
		execute_full_command(tree->pipe->right, env);
		exit(1);
	}
	close(p[0]);
	close(p[1]);
	waitpid(left, NULL, 0);
	waitpid(right, NULL, 0);
}
