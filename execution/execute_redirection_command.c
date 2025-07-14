/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redirection_command.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 14:00:40 by mbounoui          #+#    #+#             */
/*   Updated: 2025/07/14 16:16:53 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <fcntl.h>
#include <readline/history.h>
#include <sys/wait.h>
#include <unistd.h>

int	check_if_exist(t_redirection *node)
{
	int	i;

	i = 0;
	while (node->in_files[i])
	{
		if (access(node->in_files[i], F_OK))
		{
			ft_putstr_fd("minishell : ", 2);
			ft_putstr_fd(node->in_files[i], 2);
			ft_putstr_fd(": no such file or directory :", 2);
			ft_putstr_fd("\n", 2);
			return 0;
		}
		i++;
	}
	return (1);
}

void	close_fds(int *list)
{
	int	i;

	i = 0;
	while (list[i])
	{
		close(list[i]);
		i++;
	}
}

void	dup_fds(t_redirection *node, t_env *env, char **envp)
{
	(void)env;
	(void)envp;
	// 2 check flags of stdin (input file or heredoc)
	//		if herdoc
	//			dup2(last_fd, 0);
	//			close(last_fd);
	//		else
	//			fd = open("in_file");
	//			dup2(fd, 0);
	//			close(fd);
	//
	// 3 check type of stdout (trunk or append)
	//		if (trunk)
	//			fd = open("out_file", O_CREAT | O_TRUNK);
	//		else if (append)
	//			fd = open("out_file", O_CREAT | O_APPEND);
	int	in_fd;
	int	out_fd;

	if (node->hdc)
	{
		dup2(node->last_fd, 0);
		close_fds(node->heredoc_fds);
	}
	else if (node->in)
	{
		if ((in_fd = open(node->in_file, O_RDONLY)) == -1)
			exit(-1);
		dup2(in_fd, 0);
		close(in_fd);
		close_fds(node->heredoc_fds);
	}
	if (node->out_type == R_APPEND)
	{
		if ((out_fd = open(node->out_file, O_CREAT | O_APPEND, 0777)) == -1)
			exit(-1);
		dup2(out_fd, 1);
	}
	else if (node->out_type == R_OUT)
	{
		if ((out_fd = open(node->out_file, O_CREAT | O_TRUNC, 0777)) == -1)
			exit(-1);
		dup2(out_fd, 1);
	}
}



void	execute_redirection_command(t_tree *node, t_env *env, char **envp)
{
	(void)env;
	(void)envp;
	pid_t pid;

	// 1 check input file if is exist;
	if (!check_if_exist(node->redirect))
		return ;
	pid = fork();
	if (pid == 0)
	{
		// duplicate fds
		dup_fds(node->redirect, env, envp);
	//	char *path = find_path(node , env);
	//	execve(path, node->command->args, envp);
	//	perror("execve");
	//	exit(1);
		//child	process;
	}
	else if (pid > 0)
	{
		int status;
		waitpid(pid, &status, 0);
//		ret = WEXITSTATUS(status);
		//parrent process;
	}
}

