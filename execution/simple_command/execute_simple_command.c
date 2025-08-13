/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_simple_command.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moraouf <moraouf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 14:40:47 by mbounoui          #+#    #+#             */
/*   Updated: 2025/08/13 17:58:45 by moraouf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	**env_list_to_array(t_env *env)
{
	int		count;
	t_env	*tmp;
	char	**envp;
	char	*temp;
	int		i;

	count = 0;
	tmp = env;
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	envp = ft_malloc(sizeof(char *), (count + 1));
	tmp = env;
	i = 0;
	while (i < count)
	{
		temp = ft_strjoin(tmp->key, "=");
		envp[i++] = ft_strjoin(temp, tmp->value);
		free(temp);
		tmp = tmp->next;
	}
	envp[count] = NULL;
	return (envp);
}

void	protect(t_env *env, char *message)
{
	ft_putendl_fd(message, 2);
	ft_free_garbage(ft_function());
	free_env(env);
	exit(1);
}

void	child_process(t_tree *node, t_env *env)
{
	char	*path;
	int		flag;

	flag = 0;
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	empty_command(node, env);
	dote_command(node, env);
	absolute_path(node, env);
	path = find_path(node, env, &flag);
	if (!path && !flag)
		print_and_exit(node, env, 127, ": command not found");
	else if (flag)
		print_and_exit(node, env, 126, ": Permisson denied");
	execve(path, node->command->args, env_list_to_array(env));
	free_env(env);
	ft_free_garbage(ft_function());
	perror("minishell");
	exit(127);
}

void	parent_process(t_env *env, int status, pid_t pid)
{
	int	sige;

	sige = WTERMSIG(status);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	if (waitpid(pid, &status, 0) == -1)
		protect(env, "Waitpid failed");
	if (WIFSIGNALED(status))
	{
		sige = WTERMSIG(status);
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

void	execute_command_node(int i, t_tree *node, t_env **env)
{
	int		status;
	pid_t	pid;

	status = 0;
	sig_ctrl(1);
	if (is_builtin(node->command->command))
	{
		execute_builtin(i, node, env);
		return ;
	}
	pid = fork();
	if (pid < 0)
		protect(*env, "Fork failed");
	else if (pid == 0)
		child_process(node, *env);
	else
		parent_process(*env, status, pid);
}
