/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_simple_command.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moraouf <moraouf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 14:40:47 by mbounoui          #+#    #+#             */
/*   Updated: 2025/07/29 10:55:52 by mbounoui         ###   ########.fr       */
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
		return (echo_command(node->command->args));
	else if (ft_strncmp(command, "pwd", 4) == 0)
		return (pwd_command());
	else if (ft_strncmp(command, "export", 7) == 0)
		return (export_command(env, node->command->args));
	else if (ft_strncmp(command, "unset", 6) == 0)
		return (unset_command(env, node->command->args));
	else if (ft_strncmp(command, "env", 4) == 0)
		return (env_command(env, node->command->args));
	else if (ft_strncmp(command, "exit", 5) == 0)
		return (exit_command(node, env, node->command->args));
	
	return (1); // Should not reach here
}

void	print_and_exit(char *command, char *message, int code)
{
	print_message(command, message);
	global(code);
}

void	command_is_directory(t_env *env, char *command)
{
	(void)env;
	DIR	*dir;

	dir = opendir(command);
	if (!dir && command[0] == '/')
	{
		print_and_exit(command, ": No such file or directory", 127);
		free_env(env);
		ft_free_garbage(ft_function());
		exit(127);
	}
	closedir(dir);
	if ((dir = opendir(command)))
	{
		if (ft_strchr(command, '/'))
		{
			print_and_exit(command, ": Is a directory", 126);
			closedir(dir);
			free_env(env);
			ft_free_garbage(ft_function());
			exit(126);
		}
		else
		{
			print_and_exit(command, ": command not found", 127);
			closedir(dir);
			free_env(env);
			ft_free_garbage(ft_function());
			exit(127);
		}
	}
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
			print_and_exit(node->command->command, ": Permission denied", 126);
			free_env(env);
			ft_free_garbage(ft_function());
			exit(126);
		}
	}
	else
	{
		print_and_exit(node->command->command, ": No such file or directory", 127);
		free_env(env);
		ft_free_garbage(ft_function());
		exit(127);
	}
}

void	empty_command(t_tree *node, t_env *env)
{
	if (node->command->command[0] == '\0')
	{
		print_and_exit(node->command->command, ": command not found", 127);
		free_env(env);
		ft_free_garbage(ft_function());
		exit(127);
	}
}

void	find_path_and_exec(t_tree *node, t_env *env ,char **envp)
{
	char *path = find_path(node , env);
	if (!path)
	{
		print_and_exit(node->command->command, ": command not found", 127);
		free_env(env);
		ft_free_garbage(ft_function());
		exit(127);
	}
	execve(path, node->command->args, envp);
	free_env(env);
	free(path);
	ft_free_garbage(ft_function());
	perror("minishell");
	exit(127);
}

void	child_process(t_tree *node, t_env *env, char **envp)
{
	DIR *dir;
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (node->command->command[0] == '.')
	{
		if ((dir = opendir(node->command->command))) // check executable in directory
		{
			print_and_exit(node->command->command, ": Is a directory", 126);
			closedir(dir);
			//free_tree(&node);
			free_env(env);
			ft_free_garbage(ft_function());
			exit(126);
		}
		else // check if exist
			command_inside_directory(node, envp, env);
	}
	command_is_directory(env, node->command->command);
	empty_command(node, env); // ""
	find_path_and_exec(node, env ,envp);
}

void	parent_process(int status, pid_t pid, t_env *env)
{
	(void)env;

	int sig;

	sig = WTERMSIG(status);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status))
	{
		int sig = WTERMSIG(status);
		if (sig == SIGINT)
		{
			printf("\n");
			global(130);
		}
		else if (sig == SIGQUIT)
		{
			ft_putendl_fd("Quit (core dumped)", 2);
			global(131);
		}
	}
	else if (WIFEXITED(status))
		global(WEXITSTATUS(status));
	ft_free_garbage(ft_function());
}

void	execute_command_node(t_tree *node, t_env *env, char **envp)
{
	int status;

	status = 0;
	sig_ctrl(1); // start execution mode
	if (is_builtin(node->command->command))
	{
		execute_builtin(node, env);
		return ;
	}
	pid_t pid = fork();
	if (pid == 0)
		child_process(node, env, envp);
	else if (pid > 0)
		parent_process(status, pid, env);
}
