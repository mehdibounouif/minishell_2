#include "../../includes/minishell.h"

char	*ft_strdup1(const char *s)
{
	size_t	i;
	char	*res;
	size_t	len;

	if(!s)
		return NULL;
	len = ft_strlen(s) + 1;
	res = malloc(sizeof(char) * len);
	if (!res)
		return (NULL);
	i = 0;
	while (s[i])
	{
		res[i] = s[i];
		i++;
	}
	res[i] = '\0';
	return (res);
}

char	*ft_substr1(char const *s, unsigned int start, size_t len)
{
	char	*substr;
	size_t	i;
	size_t	exact_len;

	i = 0;
	if (!s)
		return (NULL);
	if (start >= ft_strlen(s))
		return ((char *)malloc(sizeof(char)));
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

char	*ft_strjoin1(char const *s1, char const *s2)
{
	char	*str;
	size_t	s1_len;
	size_t	s2_len;
	size_t	totlen;

	if (s1 && !s2)
		return (ft_strdup1(s1));
	if (!s1 && s2)
		return (ft_strdup1(s2));
	if (!s1 && !s2)
		return (0);
	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	totlen = s1_len + s2_len;
	str = malloc(sizeof(char) * totlen + 1);
	if (!str)
		return (NULL);
	ft_memcpy(str, s1, s1_len);
	ft_memcpy(str + s1_len, s2, s2_len);
	str[totlen] = '\0';
	return (str);
}