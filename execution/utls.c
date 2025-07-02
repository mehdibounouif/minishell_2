#include "../includes/minishell.h"

char	*ft_getenv(char *key, char **env)
{
	size_t	j;
	int		i;
	char	*sub;

	i = 0;
	j = ft_strlen(key);
	while (env[i])
	{
		sub = ft_substr(env[i], 0, j);
		if (ft_strncmp(sub, key, j) == 0)
		{
			free(sub);
			return (env[i] + j);
		}
		free(sub);
		i++;
	}
	return (NULL);
}

char	*find_path(t_tree *node, char **env)
{
	int		i;
	char	**all_paths;
	char	*path_slash;
	char	*full_path;

	all_paths = ft_split(ft_getenv("PATH", env), ':');
	i = 0;
	while (all_paths[i])
	{
		path_slash = ft_strjoin(all_paths[i], "/");
		full_path = ft_strjoin(path_slash, node->command->command);
		if (access(full_path, F_OK | X_OK) == 0)
		{
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_str(all_paths);
	return (NULL);
}
