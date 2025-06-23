#include "../includes/minishell.h"

int execute_full_command(t_tree *node, char **env)
{
    if (!node)
        return 1;

    if (node->type == COMMAND_NODE)
	    return execute_command_node(node, env);
    return (1);
}
