/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redirection_command.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 14:00:40 by mbounoui          #+#    #+#             */
/*   Updated: 2025/07/19 11:17:22 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <dirent.h>
#include <stdio.h>
#include <unistd.h>

int	ft_arraylen(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
		i++;
	return (i);
}

int	len_slash(char *str, char c, int len)
{
	while(str[len] != c)
		len--;
	return (len);
}

void	print_message(char *file, char *message)
{
	ft_putstr_fd("minishell : ", 2);
	ft_putstr_fd(file, 2);
	ft_putendl_fd(message, 2);
}

int	check_infile_in_directory(char *files)
{
	DIR *dir;
	char *full_path;
	int l;

	l = 0;
	if (ft_strchr(files, '/'))
	{
		l = len_slash(files, '/', ft_strlen(files) - 1);
		full_path = ft_substr(files, 0, l);
		dir = opendir(full_path);
		if (!dir)
		{
			free(full_path);
			print_message(files, ": No such file or directory");
			global(1);
			return 0;
		}
		else
		{
			free(full_path);
			closedir(dir);
			if (access(files, F_OK))
			{
				print_message(files, ": No such file or directory");
				global(1);
				return 0;
			}
		}
	}
	return (1);
}

int	check_outfile_in_directory(char *files)
{
	DIR *dir;
	char *full_path;
	int l;

	l = 0;
	if (ft_strchr(files, '/'))
	{
		l = len_slash(files, '/', ft_strlen(files) - 1);
		full_path = ft_substr(files, 0, l);
		dir = opendir(full_path);
		if (!dir)
		{
			free(full_path);
			print_message(files, ": No such file or directory");
			global(1);
			return 0;
		}
		else
		{
			free(full_path);
			closedir(dir);
		}
	}
	return (1);
}

int	check_in_files(char **files)
{
	DIR *dir;
	int	i;

	i = 0;
	while (files[i])
	{
		// if is directory do noting
		if ((dir = opendir(files[i])))
		{
			closedir(dir);
			global(1);
			return (0);
		}
		// check if the stdin file is inside path of directorys then check if
		// the directorys is exist
		if (!check_infile_in_directory(files[i]))
			return (0);
		//check the stdin file if exist
		if (access(files[i], F_OK))
		{
			print_message(files[i], ": No such file or directory");
			global(1);
			return 0;
		}
		i++;
	}
	return (1);
}

int	check_if_exist(t_redirection *node)
{
	DIR	*dir;
	int i;
	// check in files 
	if (!check_in_files(node->in_files))
		return (0);
	i = 0;
	// check out files
	while (node->out_files[i])
	{
		// check if the full path is directory (error)
		if ((dir = opendir(node->out_files[i])))
		{
			closedir(dir);
			print_message(node->out_files[i],": Is a directory");
			global(1);
			return 0;
		}
		// check if the path has directories then split it and check
		// the the path without the file name if exist or not
		check_outfile_in_directory(node->out_files[i]);
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
	// check executable
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

