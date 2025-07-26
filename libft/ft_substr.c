/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbounoui <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 19:00:47 by mbounoui          #+#    #+#             */
/*   Updated: 2025/07/26 21:58:08 by mbounoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "../includes/minishell.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*substr;
	size_t	i;
	size_t	exact_len;

	i = 0;
	if (!s)
		return (NULL);
	if (start >= ft_strlen(s))
		return ((char *) ft_malloc(1, sizeof(char)));
	if (ft_strlen(s) <= start + len)
		exact_len = ft_strlen(s) - start;
	else
		exact_len = len;
	substr = malloc(sizeof(char) * exact_len + 1);
	if (!substr)
		return (NULL);
	while (s[start] && i < len)
		substr[i++] = s[start++];
	substr[i] = '\0';
	return (substr);
}
