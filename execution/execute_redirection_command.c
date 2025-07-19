/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redirection_command.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 14:00:40 by mbounoui          #+#    #+#             */
/*   Updated: 2025/07/19 09:18:11 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <dirent.h>
#include <stdio.h>

int	ft_arraylen(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
		i++;
	return (i);
}

int	check_in_files(char **files)
{
	int	i;
	int	j;
	char	**folders;
	int	len;

	// if is directory do noting
	i = 0;
	j = 0;
	while (files[i])
	{
		j = 0;
		len = ft_strlen(files[i]);
		if (ft_strchr(files[i], '/'))
		{
			folders = ft_split(files[i], '/');
			while (folders[j])
			{
				if (access(folders[j], F_OK))
				{
					ft_putstr_fd("minishell : ", 2);
					ft_putstr_fd(files[i], 2);
					ft_putstr_fd(": No such file or directory", 2);
					ft_putstr_fd("\n", 2);
					global(1);
					return 0;
				}
				j++;
			}
		}
		else
			if (access(files[i], F_OK))
			{
				ft_putstr_fd("minishell : ", 2);
				ft_putstr_fd(files[i], 2);
				ft_putstr_fd(": No such file or directory :", 2);
				ft_putstr_fd("\n", 2);
				global(1);
				return 0;
			}
		i++;
	}
	return (1);
}

int	len_slash(char *str, char c, int len)
{
	while(str[len] != c)
		len--;
	return (len);
}

int	check_if_exist(t_redirection *node)
{
	int	i;
	int	j;
	int	l;
	char	*full_path;
	DIR	*dir;
	int	len;
	if (!check_in_files(node->in_files))
		return (0);
	i = 0;
	while (node->out_files[i])
	{
		// check if the full path is directory (error)
		j = 0;
		len = ft_strlen(node->out_files[i]);
		if ((dir = opendir(node->out_files[i])))
		{
			closedir(dir);
			ft_putstr_fd("minishell : ", 2);
			ft_putstr_fd(node->out_files[i], 2);
			ft_putendl_fd(": Is a directory", 2);
			global(1);
			return 0;
		}
		// check if the path has directories then split it and check
		// the the path without the file name if exist or not
		if (ft_strchr(node->out_files[i], '/'))
		{
			l = len_slash(node->out_files[i], '/', ft_strlen(node->out_files[i]));
			full_path = ft_substr(node->out_files[i], 0, l);
			if (!(dir = opendir(full_path)))
			{
				free(full_path);
				ft_putstr_fd("minishell : ", 2);
				ft_putstr_fd(node->out_files[i], 2);
				ft_putendl_fd(": No such file or directory", 2);
				global(1);
				return 0;
			}
			else
			{
				free(full_path);
				closedir(dir);
			}
		}
		i++;
	}
	return (1);
}

void	close_fds(int *list)
{
	int	i;

	i = 0;
	while (list[i] != 0)
	{
		close(list[i]);
		i++;
	}
}

void	dup_fds(t_redirection *node, t_env *env, char **envp)
{
	(void)env;
	(void)envp;

	int	in_fd;
	int	out_fd;
	if (node->hdc)
	{
		if ((in_fd = open(node->hrc_file, O_RDONLY)) == -1)
			exit(-1);
		dup2(in_fd, 0);
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
		if ((out_fd = open(node->out_file, O_WRONLY | O_CREAT | O_APPEND, 0777)) == -1)
			exit(-1);
		dup2(out_fd, 1);
	}
	else if (node->out_type == R_OUT)
	{
		if ((out_fd = open(node->out_file,O_WRONLY | O_CREAT | O_TRUNC, 0777)) == -1)
			exit(-1);
		printf("%d", dup2(out_fd, 1));
	}
}

void	execute_redirection_command(t_tree *node, t_env *env, char **envp)
{
	(void)env;
	(void)envp;
	pid_t pid;
	int	status;

	// 1 check input file if is exist;
	if (!check_if_exist(node->redirect))
		return ;
	pid = fork();
	if (pid == 0)
	{
		// duplicate fds
		dup_fds(node->redirect, env, envp);
		char *path = find_path(node->redirect->prev , env);
		if (!path)
		{
			ft_putstr_fd(node->redirect->prev->command->command, 2);
			ft_putendl_fd(": command not found", 2);
			global(127);
		}
		execve(path, node->redirect->prev->command->args, envp);
		ft_putstr_fd(node->redirect->prev->command->command, 2);
		ft_putendl_fd(": command not found", 2);
		global(126);
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		global(WEXITSTATUS(status));
	}
}

