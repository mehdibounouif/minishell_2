#include "../includes/minishell.h"

int execute_command_node(t_tree *node)
{
    pid_t pid = fork();
    if (pid == 0)
    {
        // Child runs the command
        execvp(node->command->command, node->command->args);
        perror("execvp"); // if it fails
        exit(1);
    }
    else
    {
        // Parent waits
        int status;
        waitpid(pid, &status, 0);
        return WEXITSTATUS(status);
    }
}
