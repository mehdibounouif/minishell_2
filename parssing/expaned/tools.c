/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 14:09:23 by mbounoui          #+#    #+#             */
/*   Updated: 2025/07/28 15:04:15 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

typedef struct s_share2
{
	int		i;
	size_t	full_len;
	size_t	cmd_len;
	char	*value;
	char	*key;

}	t_share2;

char	*get_env_key(char *cmd, int i)
{
	int		len;
	char	*key;

	i++;
	len = get_env_len(cmd, i);
	key = ft_substr(cmd, i, len);
	return (key);
}

void	init_share(t_share2 *share, char *cmd)
{
	share->i = 0;
	share->full_len = 0;
	share->key = NULL;
	share->value = NULL;
	share->cmd_len = ft_strlen(cmd);
}

int	get_full_len(char *cmd, t_env *list)
{
	t_share2	*share;

	share = ft_malloc(sizeof(t_share2), 1);
	init_share(share, cmd);
	while (cmd[share->i])
	{
		if (is_dollar(cmd, share->i)
			&& (ft_isalpha(cmd[share->i + 1]) || cmd[share->i + 1] == '_'))
		{
			share->key = get_env_key(cmd, share->i++);
			share->value = ft_getenv(share->key, list);
			if (!share->value)
				share->value = ft_strdup("");
			share->cmd_len -= (ft_strlen(share->key) + 1);
			share->full_len += ft_strlen(share->value);
			free(share->key);
		}
		share->i++;
	}
	return (share->cmd_len += share->full_len);
}

void	expand_exit_status(t_share *share)
{
	int		exit_status;
	int		l;
	char	*value;

	exit_status = global(-1);
	value = ft_itoa(exit_status);
	l = 0;
	while (value[l])
		share->expanded_cmd[share->j++] = value[l++];
	(share->i) += 2;
}
