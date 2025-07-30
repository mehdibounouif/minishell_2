/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utls.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 10:06:24 by mbounoui          #+#    #+#             */
/*   Updated: 2025/07/30 15:29:00 by mbounoui         ###   ########.fr       */
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

int	len_slash(char *str, char c, int len)
{
	while(str[len] != c)
		len--;
	return (len);
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

char	*find_path(t_tree *node, t_env *list)
{
	int		i;
	char	**all_paths;
	char	*path_slash;
	char	*full_path;

	all_paths = ft_split(ft_getenv("PATH", list), ':');
	i = 0;
	while (all_paths[i])
	{
		path_slash = ft_strjoin(all_paths[i], "/");
		full_path = ft_strjoin(path_slash, node->command->command);
		free(path_slash);
		if (access(full_path, F_OK | X_OK) == 0)
		{
			free_str(all_paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_str(all_paths);
	return (NULL);
}
