/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_simple_command.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 14:40:47 by mbounoui          #+#    #+#             */
/*   Updated: 2025/07/20 09:55:12 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>
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
	DIR *dir;
	struct stat	info;

	if (is_builtin(node->command->command))
	{
		execute_builtin(node, env);
		return ;
	}
	pid_t pid = fork();
	if (pid == 0)
	{
		// check umprintable cammand
		if (node->command->command[0] == '.')
		{
			// check executable in directory
			if ((dir = opendir(node->command->command)))
			{
				print_message(node->command->command, ": Is a directory");
				global(126);
				return ;
			}
			else
			{
				// check if exist 
				if (!stat(node->command->command, &info))
				{
					// executable
					if (info.st_mode & (S_IXGRP | S_IXUSR | S_IXOTH))
					{
						execve(node->command->command, node->command->args, envp);
					}
					// not executable
					else
					{
						print_message(node->command->command, ": Permission denied");
						global(126);
						return ;
					}
				}
				else
				{
					print_message(node->command->command, ": No such file or directory");
					global(127);
					return ;
				}
			}
		}
		char *path = find_path(node , env);
		if (!path)
		{
			print_message(node->command->command, ": command not found");
			global(127);
			return ;
		}
		execve(path, node->command->args, envp);
		perror("minishell");
//		print_message(node->command->command, ": Permission denied");
//		global(126);
	}
	else
	{
		waitpid(pid, &status, 0);
		global(WEXITSTATUS(status));
	}
}
