/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built-in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moraouf <moraouf@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 15:31:08 by mbounoui          #+#    #+#             */
/*   Updated: 2025/08/11 00:31:00 by moraouf           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// Function to check if a command is a builtin
int	is_builtin(char *command)
{
	if (!command)
		return (0);
	if (ft_strncmp(command, "cd", 3) == 0)
		return (1);
	else if (ft_strncmp(command, "echo", 5) == 0)
		return (1);
	else if (ft_strncmp(command, "pwd", 4) == 0)
		return (1);
	else if (ft_strncmp(command, "export", 7) == 0)
		return (1);
	else if (ft_strncmp(command, "unset", 6) == 0)
		return (1);
	else if (ft_strncmp(command, "env", 4) == 0)
		return (1);
	else if (ft_strncmp(command, "exit", 5) == 0)
		return (1);
	return (0);
}

int	execute_builtin(t_tree *node, t_env **env)
{
	char	*command;

	command = node->command->command;
	if (ft_strncmp(command, "cd", 3) == 0)
		return (cd_command(*env, node->command->args));
	else if (ft_strncmp(command, "echo", 5) == 0)
		return (echo_command(node->command->args));
	else if (ft_strncmp(command, "pwd", 4) == 0)
		return (pwd_command());
	else if (ft_strncmp(command, "export", 7) == 0)
		return (export_command(env, node->command->args));
	else if (ft_strncmp(command, "unset", 6) == 0)
		return (unset_command(env, node->command->args));
	else if (ft_strncmp(command, "env", 4) == 0)
		return (env_command(*env, node->command->args));
	else if (ft_strncmp(command, "exit", 5) == 0)
		return (exit_command(node, *env, node->command->args));
	return (1);
}

int	execute_builtin_command(char *command, char **args, t_env **env)
{
	if (ft_strncmp(command, "cd", 3) == 0)
		return (cd_command(*env, args));
	else if (ft_strncmp(command, "echo", 5) == 0)
		return (echo_command(args));
	else if (ft_strncmp(command, "pwd", 4) == 0)
		return (pwd_command());
	else if (ft_strncmp(command, "export", 7) == 0)
		return (export_command(env, args));
	else if (ft_strncmp(command, "unset", 6) == 0)
		return (unset_command(env, args));
	else if (ft_strncmp(command, "env", 4) == 0)
		return (env_command(*env, args));
	else if (ft_strncmp(command, "exit", 5) == 0)
		return (exit_command(NULL, *env, args));
	return (1);
}
