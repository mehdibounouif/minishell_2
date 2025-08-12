/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moraouf <moraouf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 16:19:35 by mbounoui          #+#    #+#             */
/*   Updated: 2025/08/12 13:17:46 by moraouf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	check_infile_in_directory(char *files)
{
	DIR		*dir;
	char	*full_path;
	int		l;

	l = 0;
	if (ft_strchr(files, '/'))
	{
		l = len_slash(files, '/', ft_strlen(files) - 1);
		full_path = ft_substr(files, 0, l);
		dir = opendir(full_path);
		if (!dir)
		{
			return (print(files, ": No such file or directory", 1), 0);
		}
		else
		{
			closedir(dir);
			if (access(files, F_OK))
				return (print(files, ": No such file or directory", 1), 0);
		}
	}
	return (1);
}

int	check_in_files(char *file)
{
	DIR	*dir;

	dir = opendir(file);
	if (dir)
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
		return (0);
	}
	return (1);
}

static int	exist_check_permession_else_create(char *file, DIR *dir)
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
		fd = open(file, O_RDWR | O_CREAT, 0644);
		if (fd == -1)
		{
			perror("Open");
			closedir(dir);
			return (0);
		}
		close(fd);
	}
	return (1);
}

static int	check_correct_path(char *file, char *full_path)
{
	DIR	*dir;

	dir = opendir(full_path);
	if (!dir)
	{
		print_message(file, ": No such file or directory");
		global(1);
		return (0);
	}
	else
	{
		if (!exist_check_permession_else_create(file, dir))
		{
			closedir(dir);
			return (0);
		}
		closedir(dir);
	}
	return (1);
}

int	in_directory(char *file)
{
	char	*full_path;
	int		l;

	l = len_slash(file, '/', ft_strlen(file) - 1);
	full_path = ft_substr(file, 0, l);
	if (!check_correct_path(file, full_path))
		return (0);
	return (1);
}
