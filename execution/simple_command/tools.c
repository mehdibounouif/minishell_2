/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taha_laylay <taha_laylay@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 16:40:20 by mbounoui          #+#    #+#             */
/*   Updated: 2025/08/11 15:45:49 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	print(char *command, char *message, int code)
{
	print_message(command, message);
	global(code);
}

void	print_and_exit(t_tree *node, t_env *env, int code, char *message)
{
	print(node->command->command, message, code);
	free_env(env);
	ft_free_garbage(ft_function());
	exit(code);
}

void	empty_command(t_tree *node, t_env *env)
{
	if (node->command->command[0] == '\0')
		print_and_exit(node, env, 127, ": command not found");
}

void	dote_command(t_tree *node, t_env *env)
{
	if (node->command->command[0] == '.' && !node->command->command[1])
	{
		print(node->command->command, ": filename argument required", 2);
		ft_putendl_fd(".: usage: . filename [arguments]", 2);
		free_env(env);
		ft_free_garbage(ft_function());
		exit(global(-1));
	}
	else if (node->command->command[0] == '.'
		&& !ft_strchr(node->command->command, '/'))
		print_and_exit(node, env, 127, ": command not found");
}

void	absolute_path(t_tree *node, t_env *env, char **envp)
{
	struct stat	st;
	char		*command;

	command = node->command->command;
	if (ft_strchr(command, '/')
		|| (command[0] == '.' && (command[1] == '/' || command[1] == '\0')))
	{
		if (stat(command, &st) == -1)
			print_and_exit(node, env, 127, ": No such file or directory");
		if (S_ISDIR(st.st_mode))
			print_and_exit(node, env, 126, ": Is a directory");
		if (access(command, X_OK) == -1)
			print_and_exit(node, env, 126, ": Permission denied");
		execve(command, node->command->args, envp);
		perror("minishell");
		free_env(env);
		ft_free_garbage(ft_function());
		exit(127);
	}
}
