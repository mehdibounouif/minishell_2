/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utls.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moraouf <moraouf@student.42.fr>              +#+  +:+      
	+#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 10:06:24 by mbounoui          #+#    #+#             */
/*   Updated: 2025/08/12 01:13:41 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <sys/stat.h>

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

typedef struct s_find_path
{
	int			i;
	char		**all;
	char		*slash;
	char		*full;
	int			idx;
	struct stat	st;

}	t_find_path;

char	*get_path(t_tree *node, t_find_path *share, int *flag)
{
	int		i;

	i = 0;
	while (share->all[i])
	{
		share->slash = ft_strjoin(share->all[i], "/");
		share->full = ft_strjoin(share->slash, node->command->command);
		ft_memset(&share->st, 0, sizeof(struct stat));
		stat(share->full, &share->st);
		if (!access(share->full, F_OK) && !S_ISDIR(share->st.st_mode))
		{
			if (!access(share->full, X_OK))
				return (*flag = 0, share->full);
			else
			{
				share->idx = i;
				*flag = 1;
			}
		}
		i++;
	}
	return (NULL);
}

char	*find_path(t_tree *node, t_env *list, int *flag)
{
	t_find_path	*share;

	share = ft_malloc(sizeof(t_find_path), 1);
	share->all = ft_split(ft_getenv("PATH", list), ':');
	share->idx = -1;
	if (!share->all)
	{
		print(node->command->command, ":  No such file or directory", 127);
		free_env(list);
		ft_free_garbage(ft_function());
		exit(127);
	}
	if (!get_path(node, share, flag))
	{
		if (share->idx != -1)
		{
			share->slash = ft_strjoin(share->all[share->idx], "/");
			share->full = ft_strjoin(share->slash, node->command->command);
			return (share->full);
		}
		return (NULL);
	}
	return (share->full);
}
