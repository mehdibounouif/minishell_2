#include "../../includes/minishell.h"

int	is_dollar(char *cmd, int i)
{
	if (cmd[i] == '$' && cmd[i+1] && check_quotes(cmd, i) != 2
			&& cmd[i-1] != '\\')
		return (1);
	return (0);
}

int	get_env_len(char *cmd, int i)
{
	int	j;

	j = 0;
	while (cmd[i] && !is_space(cmd[i]) && !is_dollar(cmd, i) && cmd[i] != '\'' && cmd[i] != '\"')
	{
		i++;
		j++;
	}
	return (j);
}


char	*get_env_key(char *cmd, int i, int flag)
{
	int	len;
	char	*key;

	i++;

	len = 0;
	if (flag == 1)
		len = get_env_len(cmd, i);
	else if (flag == 2)
		len = 1;
	else
		len = get_special_len(cmd, i);
	if (len == -1)
		return ("failed");
	else if (len == 0)
		return (ft_strdup(""));
	else
		key = ft_substr(cmd, i, len);
	return (key);
}

/*

char	*get_env_val(char *cmd, char **env, int i)
{
	char	*key;
	char	*value;

	while (cmd[i])
	{
		if (is_dollar(cmd, i) && ft_isalpha(cmd[i+1]))
		{
			key = get_env_key(cmd, i++, 1);
			value = ft_getenv(key, env);
			free(key);
			return (value);
		}
		else if (is_dollar(cmd, i) && !ft_isalpha(cmd[i+1]))
		{
			key = get_env_key(cmd, i++, 2);
			value = get_special_value(key);
		}
                i++;
        }
	return (NULL);
}
*/

int	get_full_len(char *cmd, char **env, int ret)
{
	int	i;
	size_t	full_len;
	size_t	cmd_len;
	char	*value;
	char	*key;
	(void)ret;

	i = 0;
	full_len = 0;
	cmd_len = ft_strlen(cmd);
	while (cmd[i])
	{
		if (is_dollar(cmd, i) && ft_isalpha(cmd[i+1]))
		{
			key = get_env_key(cmd, i++, 1);
			value = ft_getenv(key, env);
			if (!value)
				value = ft_strdup("");
			cmd_len -= (ft_strlen(key) + 1);
			full_len += ft_strlen(value);
			free(key);
		}
		else if (is_dollar(cmd, i) && ft_isdigit(cmd[i+1]))
		{
			key = get_env_key(cmd, i++, 2);
			value = get_special_value(key, ret);
			cmd_len -= (ft_strlen(key) + 1);
			full_len += ft_strlen(value);
			free(key);
		}
		else if (is_dollar(cmd, i) && !ft_isalpha(cmd[i+1]) && !ft_isalpha(cmd[i]))
		{
			key = get_env_key(cmd, i++, 3);
			if (ft_strncmp("failed", key, 6) == 0)
				return (-1);
			value = get_special_value(key, ret);
			if (ft_strncmp("failed", key, 6) == 0)
			{
				free(key);
				return (-1);
			}
			cmd_len -= (ft_strlen(key) + 1);
			full_len += ft_strlen(value);
			free(key);
		}
		i++;
        }
	return (cmd_len += full_len);
}

char	*replace_key_with_value(char *cmd, char **env, int ret)
{
	int	full_len;
	char	*expanded_cmd;
	char	*value;
	char	*key;
	int	i;
	int	j;
	int	l;
	(void)ret;

	i = 0;
	j = 0;
	full_len = get_full_len(cmd, env, ret);
	if (full_len == -1)
		return (NULL);
	expanded_cmd = malloc(full_len + 1);
	if (!expanded_cmd)
		return (NULL);
	while (cmd[i])
	{
		l = 0;
		if (is_dollar(cmd, i) && ft_isalpha(cmd[i+1]))
		{
			key = get_env_key(cmd, i, 1);
			value = ft_getenv(key, env);
			if (!value)
				value = ft_strdup("");
			while (value[l])
				expanded_cmd[j++] = value[l++];
			i += (ft_strlen(key) + 1);
		}
		else if (is_dollar(cmd, i) && !ft_isalpha(cmd[i+1]))
		{
			if (ft_isdigit(cmd[i+1]))
				key = get_env_key(cmd, i, 2);
			else
				key = get_env_key(cmd, i, 3);
			value = get_special_value(key, ret);
			while (value[l])
				expanded_cmd[j++] = value[l++];
			i += (ft_strlen(key) + 1);
		}
		if (!is_dollar(cmd, i))
			expanded_cmd[j++] = cmd[i++];
	}
	return (expanded_cmd);
}
