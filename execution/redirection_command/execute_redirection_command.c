/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redirection_command.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moraouf <moraouf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 14:00:40 by mbounoui          #+#    #+#             */
/*   Updated: 2025/07/30 16:31:36 by mbounoui         ###   ########.fr       */
/*   Updated: 2025/07/30 11:54:07 by moraouf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ulink_files(char **files)
{
	int	 i;

	i = 0;
	while (files[i])
	{
		unlink(files[i]);
		i++;
	}
}

void	child_process_redi(t_tree *node, t_env *env, char **envp)
{
	char *path;
	dup_fds(node->redirect);
	path = find_path(node->redirect->prev , env);
	if (!path)
	{
		ft_putstr_fd(node->redirect->prev->command->command, 2);
		ft_putendl_fd(": command not found", 2);
		ulink_files(node->redirect->heredocs);
		global(127);
		free_env(env);
		ft_free_garbage(ft_function());
		exit(127);
	}
	execve(path, node->redirect->prev->command->args, envp);
	ft_putstr_fd(node->redirect->prev->command->command, 2);
	ft_putendl_fd(": command not found", 2);
	ulink_files(node->redirect->heredocs);
	free_env(env);
	ft_free_garbage(ft_function());
	global(126);
}

void	built_in(t_tree *node, t_env *env, char *cmd)
{
	int saved_stdin;
	int saved_stdout;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	dup_fds(node->redirect); 
	int result = execute_builtin_command(cmd, node->redirect->prev->command->args, env);
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
	global(result);
	global(result);
	ft_free_garbage(ft_function());
}

void	fork_and_exec(t_tree *node, t_env *env, char **envp)
{
	pid_t pid;
	int	status;

	pid = fork();
	if (pid == 0)
		child_process_redi(node, env, envp);
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			global(WEXITSTATUS(status));
		else if (WIFSIGNALED(status))
			global(128 + WTERMSIG(status));
		ft_free_garbage(ft_function());
	}

}

void	execute_redirection_command(t_tree *node, t_env *env, char **envp)
{
	char	*cmd;

	if (!check_if_exist(node->redirect))
		return (ft_free_garbage(ft_function()));
	if (!node->redirect->prev)
	{
		check_if_exist(node->redirect);
		return (ft_free_garbage(ft_function()));
	}
	cmd = node->redirect->prev->command->command;
	if (is_builtin(cmd))
		built_in(node, env, cmd);
	else
		fork_and_exec(node, env, envp);
}
