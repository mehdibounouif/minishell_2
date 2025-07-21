/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redirection_command.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 14:00:40 by mbounoui          #+#    #+#             */
/*   Updated: 2025/07/21 10:02:35 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

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

int	exist_check_permession_else_create(t_redirection *node, int i, char *full_path, DIR *dir)
{
	int	fd;

	if (!access(node->out_files[i], F_OK))
	{
		if (access(node->out_files[i], W_OK))
		{
			print_message(node->out_files[i], ": Permission denied");
			global(1);
			return (0);
		}
	}
	else
	{
		if ((fd = open(node->out_files[i], O_RDWR | O_CREAT, 0644)) == -1)
		{
			perror("Open");
			free(full_path);
			closedir(dir);
			return (0);
		}
		node->fds_list[node->index] = fd;
		node->index++;

	}
	return (1);
}

int	in_directory(t_redirection *node, int i)
{
	DIR *dir;
	char *full_path;
	int l;

	l = len_slash(node->out_files[i], '/', ft_strlen(node->out_files[i]) - 1);
	full_path = ft_substr(node->out_files[i], 0, l);
	// incorect path
	if (!(dir = opendir(full_path)))
	{
		free(full_path);
		print_message(node->out_files[i], ": No such file or directory");
		global(1);
		return 0;
	}
	// correct path
	else
	{
		if (!exist_check_permession_else_create(node, i, full_path, dir))
			return (0);
		free(full_path);
		closedir(dir);
	}
	return (1);
}

int	is_directory(char *file)
{
	DIR	*dir;
	if ((dir = opendir(file)))
	{
		closedir(dir);
		print_message(file,": Is a directory");
		global(1);
		return 0;
	}
	return (1);
}

int	just_file(t_redirection *node, int i)
{
	if (!access(node->out_files[i], F_OK))
	{
		if (access(node->out_files[i], W_OK))
		{
			print_message(node->out_files[i],": Permission denied");
			global(1);
			return 0;
		}
	}
	else
	{
		node->fds_list[node->index] = open(node->out_files[i], O_CREAT);
		if (node->fds_list[node->index] == -1)
		{
			perror("Open");
			return 0;
		}
		node->index++;
	}
	return (1);
}

int	check_out_files(t_redirection *node)
{
	int i;

	i = 0;
	node->index = 0;
	while (node->out_files[i])
	{
		if (!is_directory(node->out_files[i]))
			return (0);
		else if (ft_strchr(node->out_files[i], '/'))
		{
			if (!in_directory(node, i))
				return (0);
		}
		else
			if (!just_file(node, i))
				return (0);
		i++;
	}
	return (1);
}

int	check_if_exist(t_redirection *node)
{
	// check in files
	if (!check_in_files(node->in_files))
		return (0);
	// check out files
	if (!check_out_files(node))
		return (0);
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

void	print_fd(t_redirection *node)
{
	int	i = 0;
	while (node->fds_list[i] != 0)
		printf("%d\n", node->fds_list[i++]);
}

void	execute_redirection_command(t_tree *node, t_env *env, char **envp)
{
	(void)env;
	(void)envp;
	pid_t pid;
	int	status;

	if (!(node->redirect->fds_list = ft_calloc(1024, sizeof(int))))
		return ;
	// 1 check input file if is exist;
	if (!check_if_exist(node->redirect))
		return ;
//	print_fd(node->redirect);
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

