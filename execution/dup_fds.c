/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dup_fds.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 15:33:34 by mbounoui          #+#    #+#             */
/*   Updated: 2025/07/30 15:36:57 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../includes/minishell.h"

void	dup_input(t_redirection *node, t_env *env)
{
	int	in_fd;

	if (node->hdc)
	{
		if ((in_fd = open(node->hrc_file, O_RDONLY)) == -1)
		{
      perror("open");
      free_env(env);
			ft_free_garbage(ft_function());
			exit(1);
		}
    if (dup2(in_fd, 0) == -1)
    {
      perror("dup2");
      ft_free_garbage(ft_function());
      free_env(env);
	    exit(1);
    }
    ulink_files(node->heredocs);
		close(in_fd);
	}
	else if (node->in)
	{
		if ((in_fd = open(node->in_file, O_RDONLY)) == -1)
		{
      perror("open");
      free_env(env);
			ft_free_garbage(ft_function());
			exit(1);
		}
    if (dup2(in_fd, 0) == -1)
    {
      perror("dup2");
      ft_free_garbage(ft_function());
      free_env(env);
	    exit(1);
    }
		close(in_fd);
	}
}

void	dup_output(t_redirection *node, t_env *env)
{
	int	out_fd;

	if (node->out_type == R_APPEND)
	{
		if ((out_fd = open(node->out_file, O_WRONLY | O_CREAT | O_APPEND, 0777)) == -1)
		{
      perror("open");
      free_env(env);
			ft_free_garbage(ft_function());
			exit(1);
		}
		if (dup2(out_fd, 1) == -1)
    {
      perror("dup2");
      ft_free_garbage(ft_function());
      free_env(env);
	    exit(1);
    }
		close(out_fd);
	}
	else if (node->out_type == R_OUT)
	{
		if ((out_fd = open(node->out_file,O_WRONLY | O_CREAT | O_TRUNC, 0777)) == -1)
		{
      perror("open");
      free_env(env);
			ft_free_garbage(ft_function());
			exit(1);
		}
    if (dup2(out_fd, 1) == -1)
    {
      perror("dup2");
      ft_free_garbage(ft_function());
      free_env(env);
	    exit(1);
    }
		close(out_fd);
	}

}

void	dup_fds(t_redirection *node, t_env *env)
{
	dup_input(node, env);
	dup_output(node, env);
}
