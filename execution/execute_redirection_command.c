#include "../includes/minishell.h"
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

void	execute_redirection_command(t_tree *node, t_env *env, char **envp)
{
	(void)env;
	(void)envp;
	int	fd;
	pid_t pid;

	if(node->redirect->redirection_type == R_OUT)
		fd = open(node->redirect->file, O_WRONLY | O_CREAT);
	else if (node->redirect->redirection_type == R_IN)
		fd = open(node->redirect->file, O_RDONLY);
	else if (node->redirect->redirection_type == R_APPEND)
		fd = open(node->redirect->file, O_WRONLY | O_APPEND);

	pid = fork();
	if (pid == 0)
	{
		if (node->redirect->redirection_type == R_OUT
		|| node->redirect->redirection_type == R_APPEND)
			dup2(fd, 1);
		else if (node->redirect->redirection_type == R_IN)
			dup2(fd, 0);
		close(fd);
	}
	else {
		int	status;
		waitpid(pid, &status, 0);
		ret = WEXITSTATUS(status);
	}
}
