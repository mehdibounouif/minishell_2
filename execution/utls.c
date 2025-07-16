#include "../includes/minishell.h"

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
		if (access(full_path, F_OK | X_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	free_str(all_paths);
	return (NULL);
}
