/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moraouf <moraouf@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 16:40:20 by mbounoui          #+#    #+#             */
/*   Updated: 2025/08/12 20:24:44 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char    **env_list_to_array(t_env *env)
{
    int        count;
    t_env    *tmp;
    char    **envp;
    size_t    len;

    count = 0;
    tmp = env;
    while (tmp)
    {
        count++;
        tmp = tmp->next;
    }
    envp = malloc(sizeof(char *) * (count + 1));
    if (!envp)
        return (NULL);
    tmp = env;
    for (int i = 0; i < count; i++)
    {
        len = strlen(tmp->key) + strlen(tmp->value) + 2;
        envp[i] = malloc(len);
        if (!envp[i])
            return (free_str(envp), NULL);
        snprintf(envp[i], len, "%s=%s", tmp->key, tmp->value);
        tmp = tmp->next;
    }
    envp[count] = NULL;
    return (envp);
}

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
	int			code;
	(void)envp;

	command = node->command->command;
	if (ft_strchr(command, '/'))
	{
		if (stat(command, &st) == -1)
		{
			if (errno == ENOENT)
				code = 127;
			else
				code = 126;
			print_and_exit(node, env, code, strerror(errno));
		}
		if (S_ISDIR(st.st_mode))
			print_and_exit(node, env, 126, ": Is a directory");
		if (access(command, X_OK) == -1)
			print_and_exit(node, env, 126, ": Permission denied");
		execve(command, node->command->args, env_list_to_array(env));
		perror("minishell");
		free_env(env);
		ft_free_garbage(ft_function());
		exit(127);
	}
}
