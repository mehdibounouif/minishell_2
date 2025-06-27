#include "../../includes/minishell.h"

char    *get_special_value(char *key, int ret)
{
	(void)ret;

	if (ft_isdigit(key[0]))
	{
		if (key[0] == '0')
			return (ft_strdup("-minishell"));
		else
			return (ft_strdup(""));
	}
	else if (key[0] == '#')
		return (ft_strdup("0"));
	else if (key[0] == '@'
		|| key[0] == '*'
		|| key[0] == '!')
		return (ft_strdup(""));
	else if (key[0] == '{')
		return (check_curly_brace(key));
	else if (key[0] == '[')
		return (check_square_bracket(key));
	else if (key[0] == '?')
		return (ft_strdup("0"));
	else if (key[0] == '$')
		return (ft_strdup("PID"));
	else
		return (ft_strdup("BA9I MAMHANDLIACH")); 
}
