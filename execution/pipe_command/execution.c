/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moraouf <moraouf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 14:40:29 by mbounoui          #+#    #+#             */
/*   Updated: 2025/08/12 13:27:03 by moraouf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	execute_full_command(t_tree *node, t_env **env, char **envp, int flag)
{
	if (!node)
		return ;
	if (node->type == COMMAND_NODE)
		execute_command_node(flag, node, env, envp);
	else if (node->type == PIPE_NODE)
		execute_pipe_node(node, *env, envp);
	else if (node->type == REDIRECT_NODE)
		execute_redirection_command(flag, node, *env, envp);
	if (flag)
	{
		free_env(*env);
		ft_free_garbage(ft_function());
	}
}
