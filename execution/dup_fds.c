/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dup_fds.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moraouf <moraouf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 15:33:34 by mbounoui          #+#    #+#             */
/*   Updated: 2025/08/12 13:17:37 by moraouf          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	herdoc(t_redirection *node, t_env *env)
{
	int	in_fd;

	in_fd = open(node->hrc_file, O_RDONLY);
	if (in_fd == -1)
		protect(env, "Open failed");
	if (dup2(in_fd, 0) == -1)
		protect(env, "Dup2 failed");
	ulink_files(node->heredocs);
	close(in_fd);
}

static void	input(t_redirection *node, t_env *env)
{
	int	in_fd;

	in_fd = open(node->in_file, O_RDONLY);
	if (in_fd == -1)
		protect(env, "Open failed");
	if (dup2(in_fd, 0) == -1)
		protect(env, "Dup2 failed");
	close(in_fd);
}

static void	appand(t_redirection *node, t_env *env)
{
	int	out_fd;

	out_fd = open(node->out_file, O_WRONLY | O_CREAT | O_APPEND, 0777);
	if (out_fd == -1)
		protect(env, "Open failed");
	if (dup2(out_fd, 1) == -1)
		protect(env, "Dup2 failed");
	close(out_fd);
}

static void	trunce(t_redirection *node, t_env *env)
{
	int	out_fd;

	out_fd = open(node->out_file, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (out_fd == -1)
		protect(env, "Open failed");
	if (dup2(out_fd, 1) == -1)
		protect(env, "Dup2 failed");
	close(out_fd);
}

void	dup_fds(t_redirection *node, t_env *env)
{
	if (node->hdc)
		herdoc(node, env);
	else if (node->in)
		input(node, env);
	if (node->out_type == R_APPEND)
		appand(node, env);
	else if (node->out_type == R_OUT)
		trunce(node, env);
}
