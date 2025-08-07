/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_simple_command.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moraouf <moraouf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 14:40:47 by mbounoui          #+#    #+#             */
/*   Updated: 2025/08/01 15:41:37 by mbounoui         ###   ########.fr       */
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
	//free(path);
	ft_free_garbage(ft_function());
	perror("minishell");
	exit(127);
}

void	child_process(t_tree *node, t_env *env, char **envp, int *p)
{
    struct stat st;

    if (p)
    {
      close(p[0]);
      close(p[1]);
    }
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);

    empty_command(node, env); // ""
    dote_command(node, env); // "." or . or ..

    // If command is absolute or relative path, try to exec directly
    if (node->command->command[0] == '/' || node->command->command[0] == '.')
    {
        if (stat(node->command->command, &st) == -1)
        {
            print(node->command->command, ": No such file or directory", 127);
            free_env(env);
            ft_free_garbage(ft_function());
            exit(127);
        }
        if (S_ISDIR(st.st_mode))
        {
            print(node->command->command, ": Is a directory", 126);
            free_env(env);
            ft_free_garbage(ft_function());
            exit(126);
        }
        if (access(node->command->command, X_OK) == -1)
        {
            print(node->command->command, ": Permission denied", 126);
            free_env(env);
            ft_free_garbage(ft_function());
            exit(126);
        }
        execve(node->command->command, node->command->args, envp);
        perror("minishell");
        free_env(env);
        ft_free_garbage(ft_function());
        exit(127);
    }
    // Otherwise, search in PATH
    find_path_and_exec(node, env, envp);
}

void	parent_process(int status, pid_t pid)
{
	int sige;

	sige = WTERMSIG(status);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	waitpid(pid, &status, 0);
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

void	execute_command_node(t_tree *node, t_env *env, char **envp, int *p)
{
	int status;

	status = 0;
	sig_ctrl(1); // start execution mode
	if (is_builtin(node->command->command))
	{
		execute_builtin(node, env);
    //ft_free_garbage(ft_function());
		return ;
	}
	pid_t pid = fork();
	if (pid == 0)
		child_process(node, env, envp, p);
	else if (pid > 0)
		parent_process(status, pid);
}
