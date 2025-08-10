/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 16:21:43 by mbounoui          #+#    #+#             */
/*   Updated: 2025/07/30 16:30:06 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	is_directory(char *file)
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

static int	just_file(char *file, int type)
{
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
	return (1);
}

static int	check_out_files(char *file, int type)
{
	if (!is_directory(file))
		return (0);
	else if (ft_strchr(file, '/'))
	{
		if (!in_directory(file))
			return (0);
	}
	else
		if (!just_file(file, type))
			return (0);
	return (1);
}

int	check_if_exist(t_redirection *node)
{
	while (node->files)
	{
		// check in files
		if (node->files->type == R_IN)
			if (!check_in_files(node->files->file))
				return (0);
		// check out files
		if (node->files->type == R_OUT || node->files->type == R_APPEND)
			if (!check_out_files(node->files->file, node->files->type))
				return (0);
		node->files = node->files->next;
	}
	return (1);
}

