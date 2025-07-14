/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redirection_command.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 14:00:40 by mbounoui          #+#    #+#             */
/*   Updated: 2025/07/14 14:00:41 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
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

void	execute_redirection_command(t_tree *node, t_env *env, char **envp)
{
	(void)env;
	(void)envp;
	//int	fd;
	//pid_t pid;
	if (!check_if_exist(node->redirect))
		return ;
/*
	pid = fork();
	if (pid == 0)
	{
		//child	process;
	}
	else if (pid > 0)
	{
		//parrent process;
	}
	else
	{
		// error pid = -1;
	}
	// 1 check input file if is exist;
	//
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
}
*/
}

