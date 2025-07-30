/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redirection_command.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moraouf <moraouf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 14:00:40 by mbounoui          #+#    #+#             */
/*   Updated: 2025/07/30 11:54:07 by moraouf          ###   ########.fr       */
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

	if ((dir = opendir(file)))
	{
		closedir(dir);
		global(1);
		return (0);
	}
	if (!check_infile_in_directory(file))
		return (0);
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
	(void)node;
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
		close(fd);
//		node->fds_list[node->index] = fd;
//		node->index++;

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
		{
			free(full_path);
			closedir(dir);
			return (0);
		}
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

int	just_file(t_redirection *node, char *file, int type)
{
	(void)node;
	int	fd;
	if (type == R_APPEND)
		fd = open(file, O_CREAT | O_APPEND, 0777);
	else
		fd = open(file, O_CREAT | O_TRUNC, 0777);
	if (fd == -1)
	{
		perror("Open");
		return 0;
	}
	close(fd);
//	node->fds_list[node->index] = open(file, O_CREAT | O_APPEND, 0777);

//	node->index++;
	return (1);
}

int	check_out_files(t_redirection *node, char *file, int type)
{
	if (!is_directory(file))
		return (0);
	else if (ft_strchr(file, '/'))
	{
		if (!in_directory(node, file))
			return (0);
	}
	else
		if (!just_file(node, file, type))
			return (0);
	return (1);
}

int	check_if_exist(t_redirection *node)
{
//	node->index = 0;
	while (node->files)
	{
		// check in files
		if (node->files->type == R_IN)
			if (!check_in_files(node->files->file))
				return (0);
		// check out files
		if (node->files->type == R_OUT)
			if (!check_out_files(node, node->files->file, node->files->type))
				return (0);
		node->files = node->files->next;
	}
	return (1);
}

void	dup_fds(t_redirection *node)
{
	int	in_fd;
	int	out_fd;

	if (node->hdc)
	{
		if ((in_fd = open(node->hrc_file, O_RDONLY)) == -1)
		{
			ft_free_garbage(ft_function());
			exit(1);
		}
		dup2(in_fd, 0);
		close(in_fd);
	}
	else if (node->in)
	{
		if ((in_fd = open(node->in_file, O_RDONLY)) == -1)
		{
			ft_free_garbage(ft_function());
			exit(1);
		}
		dup2(in_fd, 0);
		close(in_fd);
	}
	if (node->out_type == R_APPEND)
	{
		if ((out_fd = open(node->out_file, O_WRONLY | O_CREAT | O_APPEND, 0777)) == -1)
		{
			ft_free_garbage(ft_function());
			exit(1);
		}
		dup2(out_fd, 1);
		close(out_fd);
	}
	else if (node->out_type == R_OUT)
	{
		if ((out_fd = open(node->out_file,O_WRONLY | O_CREAT | O_TRUNC, 0777)) == -1)
		{
			ft_free_garbage(ft_function());
			exit(1);
		}
		dup2(out_fd, 1);
		close(out_fd);
	}
}

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
//		close_fds(node->redirect->fds_list);
		global(127);
		free_env(env);
		ft_free_garbage(ft_function());
		exit(127);
	}
	execve(path, node->redirect->prev->command->args, envp);
//	close_fds(node->redirect->fds_list);
	ft_putstr_fd(node->redirect->prev->command->command, 2);
	ft_putendl_fd(": command not found", 2);
	ulink_files(node->redirect->heredocs);
	free_env(env);
	ft_free_garbage(ft_function());
	global(126);

}

static int is_builtin_command(char *command)
{
	if (!command)
		return (0);
	
	if (ft_strncmp(command, "cd", 3) == 0)
		return (1);
	else if (ft_strncmp(command, "echo", 5) == 0)
		return (1);
	else if (ft_strncmp(command, "pwd", 4) == 0)
		return (1);
	else if (ft_strncmp(command, "export", 7) == 0)
		return (1);
	else if (ft_strncmp(command, "unset", 6) == 0)
		return (1);
	else if (ft_strncmp(command, "env", 4) == 0)
		return (1);
	else if (ft_strncmp(command, "exit", 5) == 0)
		return (1);
	
	return (0);
}

static int execute_builtin_command(char *command, char **args, t_env *env)
{
	if (ft_strncmp(command, "cd", 3) == 0)
		return (cd_command(env, args));
	else if (ft_strncmp(command, "echo", 5) == 0)
		return (echo_command(args));
	else if (ft_strncmp(command, "pwd", 4) == 0)
		return (pwd_command());
	else if (ft_strncmp(command, "export", 7) == 0)
		return (export_command(env, args));
	else if (ft_strncmp(command, "unset", 6) == 0)
		return (unset_command(env, args));
	else if (ft_strncmp(command, "env", 4) == 0)
		return (env_command(env, args));
	else if (ft_strncmp(command, "exit", 5) == 0)
	{
		// Note: exit in redirection context might need special handling
		return (exit_command(NULL, env, args));
	}
	return (1);
}

void	execute_redirection_command(t_tree *node, t_env *env, char **envp)
{
	pid_t pid;
	int saved_stdin;
	int saved_stdout;
	int	status;

	if (!check_if_exist(node->redirect))
	{
		ft_free_garbage(ft_function());
		return ;
	}
	if (node->redirect->without_cmd)
	{
		check_if_exist(node->redirect);
		return ;
	}
	char *cmd_name = node->redirect->prev->command->command;
	if (is_builtin_command(cmd_name))
	{
		saved_stdin = dup(STDIN_FILENO);
		saved_stdout = dup(STDOUT_FILENO);
		dup_fds(node->redirect); 
		int result = execute_builtin_command(cmd_name, node->redirect->prev->command->args, env);
		dup2(saved_stdin, STDIN_FILENO);
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdin);
		close(saved_stdout);
		global(result);
		global(result);
		ft_free_garbage(ft_function());
	}
	else
	{
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
}


