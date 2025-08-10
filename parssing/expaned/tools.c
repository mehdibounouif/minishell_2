/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <mbounoui@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 14:09:23 by mbounoui          #+#    #+#             */
/*   Updated: 2025/08/02 23:09:54 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

typedef struct s_share2
{
	int		i;
	size_t	full_len;
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
	share->full_len = ft_strlen(cmd);
	share->key = NULL;
	share->value = NULL;
}
int	split_len(char *content)
{
	int	 len;
	int	 i;

	i = 0;
	len = 0;
	while (is_space(content[i]))
		i++;
	while (content[i])
	{
		if (is_space(content[i]))
		{
			while (is_space(content[i]))
				i++;
			len++;
		}
		else
		{
			i++;
			len++;
		}
	}
	return (len);
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
			share->full_len -= (ft_strlen(share->key) + 1);
			share->full_len += ft_strlen(share->value);
		}  
		share->i++;
	}
	return (share->full_len + 1);
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
