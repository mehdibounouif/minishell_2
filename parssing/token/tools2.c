#include "../../includes/minishell.h"

int	contain_quoted(char *cmd, int len)
{
	int	i;
	if (!cmd)
		return (0);
	i = 0;
	while (i < len)
	{
		if (cmd[i] == '\'' || cmd[i] == '\"')
			return (1);
		i++;
	}
	return (0);
}

int	between_quoted(char *cmd, int len)
{
	if (!cmd)
		return (0);
	if (cmd[0] == '\'' && cmd[len - 1] == '\'')
		return (1);
	else if (cmd[0] == '\"' && cmd[len - 1] == '\"')
		return (2);
	else
		return (0);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] == s2[i])
	{
		if (s1[i] == '\0' && s2[i] == '\0')
			return (0);
		i++;
	}
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

int	is_qoute(char c)
{
	if (c == '\'' || c == '\"')
		return (1);
	return (0);
}

size_t	is_sep(char c)
{
	if (c == '<' || c == '>' || c == '|')
		return (1);
	return (0);
}