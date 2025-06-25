#include "../includes/minishell.h"

int	get_special_len(char *cmd, int i)
{
	int	j;

	j = i;
	if (cmd[i] == '[')
	{
		i++;
		while (cmd[i] && cmd[i] != ']' && !is_dollar(cmd, i))
			i++;
		i++;
	}
	else if (cmd[i] == '{')
	{
		i++;
		while (cmd[i] && cmd[i] != '}' && !is_dollar(cmd, i))
			i++;
		i++;
	}
	else 
		return (1);
	return (i - j);
}

char    *get_special_value(char *key, int ret)
{
	char	*word;
	int	j;
	int	f;
	int	x;
	int	i;
	(void)ret;

	j = ft_strlen(key);
	i = 0;
	if (ft_isdigit(key[i]))
	{
		if (key[i] == '0')
			return (ft_strdup("-minishell"));
		else
			return (ft_strdup(""));
	}
	else if (ft_strncmp("#", key, j) == 0)
		return (ft_strdup("0"));
	else if (ft_strncmp("@", key, j) == 0
		|| ft_strncmp("*", key, j) == 0
		|| ft_strncmp("!", key, j) == 0
		|| ft_isdigit(key[i]))
		return (ft_strdup(""));
	else if (key[i] == '{')
	{
		f = 0;
		while (key[f] && key[f] != '}')
		{
			if ((f == 0 && key[f+1] == '}')
					|| is_space(key[f]))
				return ("failed");
			f++;
		}
		return (ft_strdup(""));
	}
	else if (key[i] == '[')
	{
		x = 0;
		i++;
		while(is_space(key[i]))
			i++;
		if (ft_isdigit(key[i]))
		{
			word = malloc(j - 1);
			while (key[i] && key[i] != ']')
			{
				if (is_space(key[i]))
				{
					while(key[i] && is_space(key[i]))
						i++;
					if (key[i] != ']')
					{
						free(word);
						return ("failed"); // error like $[1234#]
					}
				}
				if (!ft_isdigit(key[i]) && key[i] != ']')
				{
					free(word);
					return ("failed"); // error like $[1234#]
				}
				if (key[i] == ']')
					break;
				word[x++] = key[i++];
			}
			word[x] = '\0';
			return (word);
		}
		else if (ft_isalpha(key[i]) || key[i] == ']')
		{
			i++;
			while (key[i] && key[i] != ']')
			{
				if (!ft_isalpha(key[i]) && !ft_isdigit(key[i]))
					return ("failed"); // error like $[aasjfkd#]
				i++;
			}
			return (ft_strdup("0"));
		}
		else 
			return ("failed");
	}
	else if (key[0] == '?')
		return (ft_strdup("0"));
	else if (key[0] == '$')
		return (ft_strdup("PID"));
	else
		return (ft_strdup("BA9I MAMHANDLIACH")); 
}
