/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redirection_command.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 14:00:40 by mbounoui          #+#    #+#             */
/*   Updated: 2025/07/24 16:02:34 by mbounoui         ###   ########.fr       */
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

int	check_in_files(char *file)
{
	DIR *dir;

	// if is directory do noting
	if ((dir = opendir(file)))
	{
		closedir(dir);
		global(1);
		return (0);
	}
	// check if the stdin file is inside path of directorys then check if
	// the directorys is exist
	if (!check_infile_in_directory(file))
		return (0);
	//check the stdin file if exist
	if (access(file, F_OK))
	{
		print_message(file, ": No such file or directory");
		global(1);
		return 0;
	}
	return (1);
}

int	exist_check_permession_else_create(t_redirection *node, char *file, char *full_path, DIR *dir)
{
	int	fd;

	if (!access(file, F_OK))
	{
		if (access(file, W_OK))
		{
			print_message(file, ": Permission denied");
			global(1);
			return (0);
		}
	}
	else
	{
		if ((fd = open(file, O_RDWR | O_CREAT, 0644)) == -1)
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

int	in_directory(t_redirection *node, char *file)
{
	DIR *dir;
	char *full_path;
	int l;

	l = len_slash(file, '/', ft_strlen(file) - 1);
	full_path = ft_substr(file, 0, l);
	// incorect path
	if (!(dir = opendir(full_path)))
	{
		free(full_path);
		print_message(file, ": No such file or directory");
		global(1);
		return 0;
	}
	// correct path
	else
	{
		if (!exist_check_permession_else_create(node, file, full_path, dir))
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

int	just_file(t_redirection *node, char *file)
{
	if (!access(file, F_OK))
	{
		if (access(file, W_OK))
		{
			print_message(file,": Permission denied");
			global(1);
			return 0;
		}
	}
	else
	{
		node->fds_list[node->index] = open(file, O_CREAT);
		if (node->fds_list[node->index] == -1)
		{
			perror("Open");
			return 0;
		}
		node->index++;
	}
	return (1);
}

int	check_out_files(t_redirection *node, char *file)
{
	if (!is_directory(file))
		return (0);
	else if (ft_strchr(file, '/'))
	{
		if (!in_directory(node, file))
			return (0);
	}
	else
		if (!just_file(node, file))
			return (0);
	return (1);
}

int	check_if_exist(t_redirection *node)
{
	node->index = 0;
	while (node->files)
	{
		// check in files
		if (node->files->type == R_IN)
			if (!check_in_files(node->files->file))
				return (0);
		// check out files
		if (node->files->type == R_OUT)
			if (!check_out_files(node, node->files->file))
				return (0);
		node->files = node->files->next;
	}
	return (1);
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
//		close_fds(node->heredoc_fds);
	}
	else if (node->in)
	{
		if ((in_fd = open(node->in_file, O_RDONLY)) == -1)
			exit(-1);
		dup2(in_fd, 0);
		close(in_fd);
//		close_fds(node->heredoc_fds);
	}
	if (node->out_type == R_APPEND)
	{
		if ((out_fd = open(node->out_file, O_WRONLY | O_CREAT | O_APPEND, 0777)) == -1)
			exit(-1);
		dup2(out_fd, 1);
		close(out_fd);
	}
	else if (node->out_type == R_OUT)
	{
		if ((out_fd = open(node->out_file,O_WRONLY | O_CREAT | O_TRUNC, 0777)) == -1)
			exit(-1);
		dup2(out_fd, 1);
		close(out_fd);
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
	if (!check_if_exist(node->redirect))
	{
		free_tree(&node);
		return ;
	}
	if (!node->redirect->prev)
	{
		check_if_exist(node->redirect);
		free_tree(&node);
		return ;
	}
	pid = fork();
	if (pid == 0)
	{
		dup_fds(node->redirect, env, envp);
		char *path = find_path(node->redirect->prev , env);
		if (!path)
		{
			ft_putstr_fd(node->redirect->prev->command->command, 2);
			ft_putendl_fd(": command not found", 2);
			close_fds(node->redirect->fds_list);
			global(127);
			exit(127);
		}
		execve(path, node->redirect->prev->command->args, envp);
		close_fds(node->redirect->fds_list);
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

