/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utls.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moraouf <moraouf@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 10:06:24 by mbounoui          #+#    #+#             */
/*   Updated: 2025/08/11 01:20:42 by moraouf           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_arraylen(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
		i++;
	return (i);
}

int	only_space(char *str)
{
	int	i;

	i = 0;
	if (!str[i])
		return (0);
	while (str[i])
		if (!is_space(str[i++]))
			return (0);
	return (1);
}

void	print_message(char *file, char *message)
{
	ft_putstr_fd("minishell : ", 2);
	ft_putstr_fd(file, 2);
	ft_putendl_fd(message, 2);
}

char	*ft_getenv(char *key, t_env *list)
{
	while (list)
	{
		if (!ft_strcmp(key, list->key))
		{
			return (list->value);
		}
		list = list->next;
	}
	return (NULL);
}

char	*find_path(t_tree *node, t_env *list, int *flag)
{
	int		i;
	char	**all_paths;
	char	*path_slash;
	char	*full_path;
	// char 	*check;

	all_paths = ft_split(ft_getenv("PATH", list), ':');
	if (!all_paths)
	{
		print(node->command->command, ":  No such file or directory", 127);
		free_env(list);
		ft_free_garbage(ft_function());
		exit(127);
	}
	i = 0;
	while (all_paths[i])
	{
		path_slash = ft_strjoin(all_paths[i], "/");
		full_path = ft_strjoin(path_slash, node->command->command);
		if (!access(full_path, F_OK))
		{
			if(!access(full_path,X_OK))
				return (full_path);
			else
				*flag = 1;
		}
		i++;
	}
	return (NULL);
}
