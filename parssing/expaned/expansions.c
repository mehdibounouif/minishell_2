/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 15:23:34 by mbounoui          #+#    #+#             */
/*   Updated: 2025/07/26 15:23:35 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int replace_key(char *cmd, char *expanded_cmd, int *i, int *j, t_env *list)
{
	char	*value;
	char	*key;
	int	f;
	int	l;

	l = 0;
	f = 0;
	key = get_env_key(cmd, *i);
	if (!key)
		return (0);
	value = ft_getenv(key, list);
	if (!value)
		value = ft_strdup("");
	if (cmd[*i-1] == '\"' && l == 0)
		f = 1;
	if (value)
	{
		while (value[l])
		{
			if (value[l] == '\'' && f == 0)
			{
				expanded_cmd[(*j)++] = '\"';
				f = 2;
			}
			expanded_cmd[(*j)++] = value[l++];
		if (value[l] == '\'' && f == 2)
			{
				expanded_cmd[(*j)++] = value[l++];
				expanded_cmd[(*j)++] = '\"';
				f = 0;
			}
		}
	}
	(*i) += (ft_strlen(key) + 1);
	free(key);
	return (1);
}

char	*expansion(char *cmd, t_env *list)
{
	int	full_len;
	char	*expanded_cmd;
	int	(i), (j), (l);

	i = 0;
	j = 0;
	l = 0;
	///quote = 0;
	full_len = get_full_len(cmd, list);
	if (full_len == -1)
		return (NULL);
	expanded_cmd = malloc(full_len + 1);
	if (!expanded_cmd)
		return (NULL);
	while (cmd[i])
	{
		if (cmd[i] == '<' && cmd[i+1] == '<')
		{
			l = 1;
			expanded_cmd[j++]  = cmd[i++];
			expanded_cmd[j++]  = cmd[i++];
			while (is_space(cmd[i]))
				expanded_cmd[j++] = cmd[i++];
			while (cmd[i] == '\'' || cmd[i] == '\"')
				i++;
		}
		while (l == 0 && is_dollar(cmd, i) && (ft_isalpha(cmd[i+1]) || cmd[i+1] == '_'))
			if (!replace_key(cmd, expanded_cmd, &i, &j, list))
				return (NULL);
		if (is_dollar(cmd, i) && cmd[i+1] == '?')
			if (!expand_exit_status(expanded_cmd, &i, &j))
				return (NULL);
		if (is_dollar(cmd, i) && !check_quotes(cmd, i) // exmaple $"HOME"
			&& (cmd[i+1] == '\'' || cmd[i+1] == '\"'))
			i++;
		else
			expanded_cmd[j++] = cmd[i++];
		if (l == 1 && cmd[i] == ' ') // << $HOME$HOME
			l = 0;
	}
	free(cmd);
	expanded_cmd[j] = '\0';
	return (expanded_cmd);
}
