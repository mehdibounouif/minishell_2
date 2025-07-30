/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_simple_command.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moraouf <moraouf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 14:40:47 by mbounoui          #+#    #+#             */
/*   Updated: 2025/07/30 16:42:04 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	find_path_and_exec(t_tree *node, t_env *env ,char **envp)
{
	char *path = find_path(node , env);
	if (!path)
	{
		print(node->command->command, ": command not found", 127);
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
			print(node->command->command, ": Is a directory", 126);
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

void	parent_process(int status, pid_t pid)
{
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
		parent_process(status, pid);
}
