/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 14:40:29 by mbounoui          #+#    #+#             */
/*   Updated: 2025/07/30 16:18:57 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	execute_full_command(t_tree *node, t_env *env, char **envp, int pipe_flag)
{
	if (!node)
		return ;

	if (node->type == COMMAND_NODE)
		execute_command_node(node, env, envp);
	else if (node->type == PIPE_NODE)
		execute_pipe_node(node, env, envp);
	else if (node->type == REDIRECT_NODE)
		execute_redirection_command(node, env, envp);
	if (pipe_flag)
	{
		free_env(env);
		ft_free_garbage(ft_function());
	}
}
