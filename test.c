#include <stdio.h>

int	is_space(char c)
{
	if ((c >= 9 && c <= 13) || c == 32)
		return (1);
	return (0);
}

void	skip_space(char *content, int *i)
{
	while (is_space(content[*i]))
		(*i)++;
}

int	split_len(char *content)
{
	int	 len;
	int	 i;

	i = 0;
	len = 0;
	skip_space(content, &i);
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
int	main()
{
	printf("len = %d\n", split_len("    mehdi    b   c  d"));
}
