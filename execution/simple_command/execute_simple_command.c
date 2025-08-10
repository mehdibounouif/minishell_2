/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_simple_command.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moraouf <moraouf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 14:40:47 by mbounoui          #+#    #+#             */
/*   Updated: 2025/08/09 16:02:31 by moraouf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void protect(t_env *env, char *message)
{
  ft_putendl_fd(message, 2);
  ft_free_garbage(ft_function());
  free_env(env);
  exit(1);
}

void	child_process(t_tree *node, t_env *env, char **envp)
{
    char *path;

    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);

    empty_command(node, env);
    dote_command(node, env);
    absolute_path(node, env, envp);
    path = find_path(node , env);
    if (!path)
      print_and_exit(node, env, 127, ": command not found");
    execve(path, node->command->args, envp);
    free_env(env);
    ft_free_garbage(ft_function());
    perror("minishell");
    exit(127);
}

void	parent_process(t_env *env, int status, pid_t pid)
{
	int sige;

	sige = WTERMSIG(status);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	if (waitpid(pid, &status, 0) == -1)
    protect(env, "Waitpid failed");
	if (WIFSIGNALED(status))
	{
		int sige = WTERMSIG(status);
		if (sige == SIGINT)
		{
			printf("\n");
			global(130);
		}
		else if (sige == SIGQUIT)
		{
			ft_putendl_fd("Quit (core dumped)", 2);
			global(131);
		}
	}
	else if (WIFEXITED(status))
		global(WEXITSTATUS(status));
}

void	execute_command_node(t_tree *node, t_env **env, char **envp)
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
  if (pid < 0)
      protect(*env, "Fork failed");
	else if (pid == 0)
		child_process(node, *env, envp);
	else
		parent_process(*env, status, pid);
}
