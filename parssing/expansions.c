#include "../includes/minishell.h"

int	is_dollar(char *cmd, int i)
{
	if (cmd[i] == '$' && check_quotes(cmd, i) != 2
			&& cmd[i-1] != '\\')
		return (1);
	return (0);
}

int	get_env_len(char *cmd, int i)
{
	int	j;

	j = 0;
	while (cmd[i] && !is_space(cmd[i]) && cmd[i] != '\'' && cmd[i] != '\"')
	{
		i++;
		j++;
	}
	return (j);
}

char	*get_env_key(char *cmd, int i)
{
	int	len;
	char	*word;
	int	j;

	j = 0;
	i++;
	len = get_env_len(cmd, i);
	word = malloc(len + 1);
	if (!word)
		return (NULL);
	while (len)
	{
		word[j++] = cmd[i++];
		len--;
	}
	word[j] = '\0';
	return (word);
}

char	*get_env_val(char *cmd, char **env, int i)
{
	char	*key;
	char	*value;

	while (cmd[i])
	{
		if (is_dollar(cmd, i))
		{
			key = get_env_key(cmd, i++);
			value = ft_getenv(key, env);
			free(key);
			return (value);
		}
                i++;
        }
	return (NULL);
}

int	get_full_len(char *cmd, char **env)
{
	int	i;
	size_t	full_len;
	size_t	cmd_len;
	char	*value;
	char	*key;

	i = 0;
	full_len = 0;
	cmd_len = ft_strlen(cmd);
	//printf("COMMAND LEN = %ld\n", cmd_len);
	while (cmd[i])
	{
		if (is_dollar(cmd, i))
		{
			key = get_env_key(cmd, i++);
			value = ft_getenv(key, env);
			//printf("key = %s, value = %s\n", key, value);
			cmd_len -= (ft_strlen(key) + 1);
			//printf("cmd - key = %ld\n", cmd_len);
			full_len += ft_strlen(value);
			//printf("value len = %ld\n", full_len);
			free(key);
		}
		i++;
        }
	return (cmd_len += full_len);
}

char	*replace_key_with_value(char *cmd, char **env)
{
	size_t	full_len;
	char	*expanded_cmd;
	char	*value;
	char	*key;
	int	i;
	int	j;
	int	l;

	i = 0;
	j = 0;
	full_len = get_full_len(cmd, env);
	expanded_cmd = malloc(full_len + 1);
	while (cmd[i])
	{
		l = 0;
		if (is_dollar(cmd, i))
		{
			key = get_env_key(cmd, i);
			value = ft_getenv(key, env);
			while (value[l])
				expanded_cmd[j++] = value[l++];
			i += (ft_strlen(key) + 1);
		}
		expanded_cmd[j++] = cmd[i++];
	}
	return (expanded_cmd);
}
