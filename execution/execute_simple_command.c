/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_simple_command.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 14:40:47 by mbounoui          #+#    #+#             */
/*   Updated: 2025/07/17 13:15:08 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// Function to check if a command is a builtin
static int is_builtin(char *command)
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

// Function to execute builtin commands
static int execute_builtin(t_tree *node, t_env *env)
{
	char *command = node->command->command;
	
	if (ft_strncmp(command, "cd", 3) == 0)
		return (cd_command(env, node->command->args));
	else if (ft_strncmp(command, "echo", 5) == 0)
		return (echo_command(env, node->command->args));
	else if (ft_strncmp(command, "pwd", 4) == 0)
		return (pwd_command(env, node->command->args));
	else if (ft_strncmp(command, "export", 7) == 0)
		return (export_command(env, node->command->args));
	else if (ft_strncmp(command, "unset", 6) == 0)
		return (unset_command(env, node->command->args));
	else if (ft_strncmp(command, "env", 4) == 0)
		return (env_command(env, node->command->args));
	else if (ft_strncmp(command, "exit", 5) == 0)
		return (exit_command(env, node->command->args));
	
	return (1); // Should not reach here
}

// Use t_env *env for builtins, char **envp for execve
void	execute_command_node(t_tree *node, t_env *env, char **envp)
{
	if (is_builtin(node->command->command))
	{
		execute_builtin(node, env);
		return ;
	}
	// External command: fork and execve
	pid_t pid = fork();
	if (pid == 0)
	{
		char *path = find_path(node , env);
		execve(path, node->command->args, envp);
		ft_putstr_fd(node->command->command, 2);
		ft_putendl_fd(": command not found", 2);
		node->ret = 127;
	}
	else
	{
		waitpid(pid, NULL, 0);
	}
}
