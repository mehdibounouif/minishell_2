#include "../includes/minishell.h"

int execute_command_node(t_tree *node, char **env)
{
	// fork to create new child process return
	// return : 0 in child
	// the child pid in the parent
	// -1 if the fork failed.
	pid_t pid = fork();

	if (pid == 0) // child process
	{
		char *path = find_path(node, env); // ls 
		// execute command
		// replaces the child process with the new
		// if success does not return anything.
        	execve(path, node->command->args, env);
        	perror("execve");
        	exit(1);
	}
    	else
    	{
		// status is variable to store exit info 
		// like
		// if child exit normally (exit() or return)
		// if child killed by a signal
		// if there is core dump
		// So the exit code is not the whole integer 
		// — it's stored in specific bits only 
		// (usually the lower 8 bits, depending on the system).
        	int status;
        	waitpid(pid, &status, 0);
		// WEXITSTATUS(status)
		// extract only the meaningful exit code bits from status
        	return WEXITSTATUS(status);
    	}
}
