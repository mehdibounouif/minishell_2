/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_simple_command.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 14:40:47 by mbounoui          #+#    #+#             */
/*   Updated: 2025/07/18 22:08:35 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <stdlib.h>
#include <unistd.h>

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
int	check_cat_files(t_command *node)
{
	int	i;
	
	i = 1;
	while (node->args[i] && ft_strchr(node->args[i], '-'))
		i++;
	while (node->args[i])
	{
		if (access(node->args[i], F_OK))
		{
			global(1);
			return (0);
		}
		i++;
	}
	return (1);
}

void	execute_command_node(t_tree *node, t_env *env, char **envp)
{
	int status;

	if (is_builtin(node->command->command))
	{
		execute_builtin(node, env);
		return ;
	}
	pid_t pid = fork();
	if (pid == 0)
	{
		if (node->command->command[0] == '.')
		{
			// check executable in directory
			// with opendir()
			if (access(node->command->command, F_OK))
			{
				ft_putstr_fd(node->command->command, 2);
				ft_putendl_fd(": command not found", 2);
				global(127);
				return ;
			}	
			if (access(node->command->command, F_OK))
			{
				ft_putstr_fd(node->command->command, 2);
				ft_putendl_fd(": command not found", 2);
				global(127);
				return ;
			}
			execve(node->command->command, node->command->args, envp);
		}
		char *path = find_path(node , env);
		if (!path)
		{
			ft_putstr_fd(node->command->command, 2);
			ft_putendl_fd(": command not found", 2);
			global(127);
			return ;
		}
		execve(path, node->command->args, envp);
		ft_putstr_fd(node->command->command, 2);
		ft_putendl_fd(": Permission denied", 2);
		global(126);
	}
	else
	{
		waitpid(pid, &status, 0);
		global(WEXITSTATUS(status));
	}
}
