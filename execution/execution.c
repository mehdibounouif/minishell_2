#include "../includes/minishell.h"

int execute_full_command(t_tree *node, t_env *env, char **envp)
{
    if (!node)
        return 1;

    if (node->type == COMMAND_NODE)
        execute_command_node(node, env, envp);
    else if (node->type == PIPE_NODE)
        execute_pipe_node(node, env, envp);
    return (1);
}
