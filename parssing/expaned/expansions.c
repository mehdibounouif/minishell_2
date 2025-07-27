/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 15:23:34 by mbounoui          #+#    #+#             */
/*   Updated: 2025/07/27 08:11:04 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	copy_word(char *value, t_share *share, int f)
{
	int	l;

	l = 0;
	while (value[l])
	{
		if (value[l] == '\'' && f == 0)
		{
			share->expanded_cmd[(share->j)++] = '\"';
			f = 2;
		}
		share->expanded_cmd[(share->j)++] = value[l++];
		if (value[l] == '\'' && f == 2)
		{
			share->expanded_cmd[(share->j)++] = value[l++];
			share->expanded_cmd[(share->j)++] = '\"';
			f = 0;
		}
	}
}

void	replace_key(char *cmd, t_share *share, t_env *list)
{
	char	*value;
	char	*key;
	int		f;
	int		l;

	l = 0;
	f = 0;
	key = get_env_key(cmd, share->i);
	value = ft_getenv(key, list);
	if (!value)
		value = ft_strdup("");
	if (cmd[share->i - 1] == '\"' && share->l == 0)
		f = 1;
	copy_word(value, share, f);
	share->i += (ft_strlen(key) + 1);
	free(key);
}

void	check_herdoc(char *cmd, t_share *share)
{
	if (cmd[share->i] == '<' && cmd[share->i + 1] == '<')
	{
		share->l = 1;
		share->expanded_cmd[share->j++] = cmd[share->i++];
		share->expanded_cmd[share->j++] = cmd[share->i++];
		while (is_space(cmd[share->i]))
			share->expanded_cmd[share->j++] = cmd[share->i++];
		while (cmd[share->i] == '\'' || cmd[share->i] == '\"')
			share->i++;
	}
}

void	expand_cmd(char *cmd, t_share *share, t_env *env)
{
	while (cmd[share->i])
	{
		check_herdoc(cmd, share);
		while (share->l == 0 && is_dollar(cmd, share->i)
			&& (ft_isalpha(cmd[share->i + 1]) || cmd[share->i + 1] == '_'))
			replace_key(cmd, share, env);
		if (is_dollar(cmd, share->i) && cmd[share->i + 1] == '?')
			expand_exit_status(share->expanded_cmd, &share->i, &share->j);
		if (is_dollar(cmd, share->i)
			&& !check_quotes(cmd, share->i)
			&& (cmd[share->i + 1] == '\'' || cmd[share->i + 1] == '\"'))
			share->i++;
		else
			share->expanded_cmd[share->j++] = cmd[share->i++];
		if (share->l == 1 && cmd[share->i] == ' ')
			share->l = 0;
	}
}

char	*expansion(char *cmd, t_env *env)
{
	int		full_len;
	t_share	*share;

	share = ft_malloc(sizeof(t_share), 1);
	share->i = 0;
	share->j = 0;
	share->l = 0;
	full_len = get_full_len(cmd, env);
	share->expanded_cmd = ft_malloc(full_len , 1);
	expand_cmd(cmd, share, env);
	free(cmd);
	share->expanded_cmd[share->j] = '\0';
	return (share->expanded_cmd);
}
