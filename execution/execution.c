/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 14:40:29 by mbounoui          #+#    #+#             */
/*   Updated: 2025/07/08 09:32:39 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	execute_full_command(t_tree *node, t_env *env, char **envp)
{
    if (!node)
        return 0;

    if (node->type == COMMAND_NODE)
        execute_command_node(node, env, envp);
    else if (node->type == PIPE_NODE)
        execute_pipe_node(node, env, envp);
//	else if (node->type == REDIRECT_NODE)
//		execute_redirection_command(node, env, envp);
    return (1);
}
