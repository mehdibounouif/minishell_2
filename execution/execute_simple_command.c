#include "../includes/minishell.h"

int execute_command_node(t_tree *node, char **env)
{
    pid_t pid = fork();

    if (pid == 0)
    {
	char *path = find_path(node, env);
        execve(path, node->command->args, env);
        perror("execve");
        exit(1);
    }
    else
    {
        int status;
        waitpid(pid, &status, 0);
        return WEXITSTATUS(status);
    }
}
