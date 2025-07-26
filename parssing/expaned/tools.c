/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 14:09:23 by mbounoui          #+#    #+#             */
/*   Updated: 2025/07/26 15:22:38 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	while (cmd[i] && (ft_isalpha(cmd[i]) || cmd[i] == '_'))
	{
		i++;
		j++;
	}
	return (j);
}


char	*get_env_key(char *cmd, int i)
{
	int	len; char	*key;

	i++;
	len = get_env_len(cmd, i);
	key = ft_substr(cmd, i, len);
	if (!key)
		return (NULL);
	return (key);
}


int	get_full_len(char *cmd, t_env *list)
{
	int	i;
	size_t	full_len;
	size_t	cmd_len;
	char	*value;
	char	*key;

	i = 0;
	full_len = 0;
	cmd_len = ft_strlen(cmd);
	key = NULL;
	while (cmd[i])
	{
		if (is_dollar(cmd, i) && (ft_isalpha(cmd[i+1]) || cmd[i+1] == '_'))
		{
			key = get_env_key(cmd, i++);
			if (!key)
				return (-1);
			value = ft_getenv(key, list);
			if (!value)
				value = ft_strdup("");
			cmd_len -= (ft_strlen(key) + 1);
			full_len += ft_strlen(value);
			free(key);
		}
		i++;
	}
	return (cmd_len += full_len);
}

int	expand_exit_status(char *expanded_cmd, int *i, int *j)
{
	int	exit_status;
	char *value;
	int	l;

	exit_status = global(-1);
	value = ft_itoa(exit_status);
	if (!value)
		return (0);
	l = 0;
	while (value[l])
		expanded_cmd[(*j)++] = value[l++];
	(*i) += 2;
	return (1);
}

