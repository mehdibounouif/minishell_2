#include "../includes/minishell.h"

int check_quotes(char *cmd, size_t i)
{
  size_t j;
  int flag;

  j = 0;
  flag = 0;
  while (cmd[j] && j != i)
  {
	  /*
    if (j > 0 && cmd[j - 1] == '\\')
    {
      j++;
      continue;
    }
    */
    if (flag == 0 && cmd[j] == '\"')
      flag = 1;
    else if (flag == 0 && cmd[j] == '\'')
      flag = 2;
    else if (flag == 1 && cmd[j] == '\"')
      flag = 0;
    else if (flag == 2 && cmd[j] == '\'')
      flag = 0;
    j++;
  }
  return (flag);
}

char	*remove_quotes(char *cmd)
{
  size_t i;
  int j;
  char  *clear_cmd;

  i = 0;
  j = 0;
  while (cmd[i])
  {
    if (cmd[i] == '\'' || cmd[i] == '\"')
      j++;
    i++;
  }
  clear_cmd = malloc((i - j) + 1);
  i = 0;
  j = 0;
  while (cmd[i])
  {
    if (cmd[i] == '\'' || cmd[i] == '\"')
      i++;
    else
      clear_cmd[j++] = cmd[i++];
  }
  clear_cmd[j] = '\0';
  return (clear_cmd);
}

char	*remove_quotes3(char *cmd)
{
  size_t i;
  int j;
  size_t len;
  char  *clear_cmd;

  i = 0;
  j = 0;
  len = ft_strlen(cmd) - 1;
  while (cmd[i])
  {
    if ((cmd[i] == '\'' || cmd[i] == '\"') && (i == 0 || i == len))
      j++;
    i++;
  }
  clear_cmd = malloc((i - j) + 1);
  i = 0;
  j = 0;
  while (cmd[i])
  {
    if ((cmd[i] == '\'' || cmd[i] == '\"') && (i == 0 || i == len))
      i++;
    else
      clear_cmd[j++] = cmd[i++];
  }
  clear_cmd[j] = '\0';
  return (clear_cmd);
}


char    *remove_quotes2(char *cmd, size_t l)
{
	size_t i;
	int j;
	char  *clear_cmd;

	clear_cmd = malloc(ft_strlen(cmd) + 1);
	i = 0;
	j = 0;
	if (cmd[i] == '\'' || cmd[i] == '\"')
		i++;
	while (cmd[i] && i != l)
	{
    		if ((cmd[i] == '\'' && check_quotes(cmd, i) != 1)
				|| (cmd[i] == '\"' && check_quotes(cmd, i) != 2))
			i++;
		else
			clear_cmd[j++] = cmd[i++];
	}
	while (cmd[i])
		clear_cmd[j++] = cmd[i++];
	clear_cmd[j] = '\0';
	return (clear_cmd);
}
