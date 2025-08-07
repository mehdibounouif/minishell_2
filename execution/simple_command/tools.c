/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 16:40:20 by mbounoui          #+#    #+#             */
/*   Updated: 2025/07/30 16:40:33 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	print(char *command, char *message, int code)
{
	print_message(command, message);
	global(code);
}

void	folder(t_env *env, char *command)
{
	DIR *dir;

	if ((dir = opendir(command)))
	{
		if (ft_strchr(command, '/'))
		{
			print(command, ": Is a directory", 126);
			closedir(dir);
			free_env(env);
			ft_free_garbage(ft_function());
			exit(126);
		}
		else
		{
			print(command, ": command not found", 127);
			closedir(dir);
			free_env(env);
			ft_free_garbage(ft_function());
			exit(127);
		}
	}
}

void	command_is_directory(t_env *env, char *command)
{
	DIR	*dir;

	dir = opendir(command);
	// Exmple /parssing
	if (!dir && command[0] == '/')
	{
		print(command, ": No such file or directory", 127);
		free_env(env);
		ft_free_garbage(ft_function());
		exit(127);
	}
	closedir(dir);
	// Example parssing or ./parssing/
	folder(env, command);
}

void	command_inside_directory(t_tree *node, char **envp, t_env *env)
{
	struct stat	info;
	if (!stat(node->command->command, &info))
	{
		if (info.st_mode & (S_IXGRP | S_IXUSR | S_IXOTH)) // executable
			execve(node->command->command, node->command->args, envp);
		else // not executable
		{
			print(node->command->command, ": Permission denied", 126);
			free_env(env);
			ft_free_garbage(ft_function());
			exit(126);
		}
	}
	else
	{
		print(node->command->command, ": No such file or directory", 127);
		free_env(env);
		ft_free_garbage(ft_function());
		exit(127);
	}
}

void	empty_command(t_tree *node, t_env *env)
{
	if (node->command->command[0] == '\0')
	{
		print(node->command->command, ": command not found", 127);
		free_env(env);
		ft_free_garbage(ft_function());
		exit(global(-1));
	}
}

void  dote_command(t_tree *node, t_env *env)
{
  if (node->command->command[0] == '.' && !node->command->command[1])
	{
		print(node->command->command, ": filename argument required", 2);
    ft_putendl_fd(".: usage: . filename [arguments]", 2);
    free_env(env);
		ft_free_garbage(ft_function());
		exit(global(-1));
	}
  else if (node->command->command[0] == '.' && !ft_strchr(node->command->command, '/'))
  {
    print(node->command->command, ": command not found", 127);
		free_env(env);
		ft_free_garbage(ft_function());
		exit(global(-1));
  }
}
