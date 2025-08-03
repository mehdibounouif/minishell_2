/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 15:23:34 by mbounoui          #+#    #+#             */
/*   Updated: 2025/08/02 23:34:25 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	replace_key(char *cmd, t_share *share, t_env *list)
{
	char	*value;
	char	*key;
	int	l;

	l = 0;
	key = get_env_key(cmd, share->i);
	value = ft_getenv(key, list);
	if (!value)
		value = ft_strdup("");
	while (value[l])
		share->expanded_cmd[(share->j)++] = value[l++];
	share->i += (ft_strlen(key) + 1);
	free(key);
}

void	split_value(char *cmd, t_share *share, t_env *list)
{
	char	*value;
	char	*key;
	int	l;

	l = 0;
	key = get_env_key(cmd, share->i);
	value = ft_getenv(key, list);
	if (!value)
		value = ft_strdup("");
	while (value[l] && is_space(value[l]))
		l++;
	while (value[l])
	{
		if (is_space(value[l]))
		{
			while (is_space(value[l]))
				l++;
			if (value[l])
				share->expanded_cmd[share->j++] = ' ';
		}
		else
			share->expanded_cmd[share->j++] = value[l++];
	}
	share->i += (ft_strlen(key) + 1);
	free(key);
}

void	expand_cmd(char *cmd, t_share *share, t_env *env, int b_q)
{
	while (cmd[share->i])
	{
		while (is_dollar(cmd, share->i) && b_q != 1
			&& (ft_isalpha(cmd[share->i + 1]) || cmd[share->i + 1] == '_'))
		{
			if (b_q == 2)
				replace_key(cmd, share, env);
			else
				split_value(cmd, share, env);
		}
		if (is_dollar(cmd, share->i) && cmd[share->i + 1] == '?' && b_q != 1)
			expand_exit_status(share);
		else
			share->expanded_cmd[share->j++] = cmd[share->i++];
	}
}

char	*expansion(char *cmd, t_env *env, int b_q)
{
	int		full_len;
	t_share	*share;

	share = ft_malloc(sizeof(t_share), 1);
	share->i = 0;
	share->j = 0;
	share->h = 0;
	full_len = get_full_len(cmd, env, b_q);
	if (full_len == 0)
		return (ft_strdup(""));
	share->expanded_cmd = ft_malloc(sizeof(char), full_len +1);
	expand_cmd(cmd, share, env, b_q);
	free(cmd);
	share->expanded_cmd[share->j] = '\0';
	return (share->expanded_cmd);
}
